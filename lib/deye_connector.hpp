/*
* Copyright (C) 2025 ZY4N <me@zy4n.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#pragma once

#include <system_error>
#include <cstdint>
#include <string_view>
#include <array>
#include <span>
#include <expected>
#include <variant>
#include <ranges>
#include <cstring>

#include <algorithm>
#include <numeric>
#include <format>

// The deye modbus protocoll sends two checksums on top of the TCP connection.
// Since TCP has its own error correction these checksums don't need to be checked
// but the checks can be enabled with the following define:
#define DEYE_REDUNDANT_ERROR_CHECKS

namespace deye
{

//----------------[ Helpers ]----------------//

namespace detail
{

template<class... Ts>
struct overloaded_lambda : Ts...
{
	using Ts::operator()...;
};

template<typename T, typename U>
struct is_container
{
	static constexpr auto value = requires(T t, const T ct) {
		{ t.size()			} -> std::integral;
		{ *std::begin(t)	} -> std::convertible_to<U&>;
		{ *std::end(t)		} -> std::convertible_to<U&>;
		{ t[0]				} -> std::convertible_to<U&>;
		{ *std::begin(ct)	} -> std::convertible_to<const U&>;
		{ *std::end(ct)		} -> std::convertible_to<const U&>;
		{ ct[0]				} -> std::convertible_to<const U&>;
	};
};

namespace tcp_socket_concepts
{

template<class T>
concept constructor = std::is_default_constructible_v<T>;

template<class T>
concept listen = requires(T socket, std::uint16_t port)
{
	/**
	 * @brief accepts tcp connection on given port, blocks until socket connected or error occurred.
	 *
	 * @param port The port to await the connection on.
	 *
	 * @return An `std::error_code` that indicates the status of the operation.
	 */
	{ socket.listen(port) } -> std::same_as<std::error_code>;
};

template<class T>
concept connect = requires(T socket, const char* host, std::uint16_t port)
{
	/**
	 * @brief connects socket to given host and port, only returns when socket connected or error occurred.
	 *
	 * @param host The host IPv4 address to connect to.
	 * @param port The host port to connect to.
	 *
	 * @return An `std::error_code` that indicates the status of the operation.
	 */
	{ socket.connect(host, port) } -> std::same_as<std::error_code>;
};

template<class T>
concept send = requires(T socket, std::span<const std::uint8_t> data)
{
	/**
	 * @brief Sends data over a connected tcp socket, blocking until data.size() bytes are sent.
	 *
	 * @param data The data to be sent, represented as a span of unsigned 8-bit integers.
	 *
	 * @return An `std::error_code` that indicates the status of the operation.
	 */
	{ socket.send(data) } -> std::same_as<std::error_code>;
};

template<class T>
concept receive = requires(T socket, std::span<std::uint8_t> data)
{
	/**
	 * @brief Receives data from a connected socket, blocking until data.size() bytes are received.
	 *
	 * @param buffer The buffer to store the received data in, represented as a span of unsigned 8-bit integers.
	 *
	 * @return An `std::error_code` that indicates the status of the operation.
	 */
	{ socket.receive(data) } -> std::same_as<std::error_code>;
};

template<class T>
concept disconnect = requires(T socket)
{
	/**
	 * @brief Shuts down both reading and writing channels and closes the socket.
	*/
	{ socket.disconnect() } -> std::same_as<std::error_code>;
};

} // tcp_socket_concepts

template<class T>
concept tcp_socket = (
	tcp_socket_concepts::constructor<T> and
	tcp_socket_concepts::connect<T> and
	tcp_socket_concepts::send<T> and
	tcp_socket_concepts::receive<T> and
	tcp_socket_concepts::disconnect<T>
);

namespace modbus
{

[[nodiscard]] inline constexpr std::uint8_t checksum(std::span<const std::uint8_t> data);

[[nodiscard]] inline constexpr std::uint16_t crc(std::span<const std::uint8_t> data);

} // namespace modbus

namespace bytes
{

template<typename T, std::endian Endian>
std::error_code from(const auto &value, std::span<std::uint8_t> bytes, std::size_t* offset);

template<typename T, std::endian Endian>
std::error_code from(const auto &value, std::span<std::uint8_t> bytes, std::size_t offset);


template<typename T, std::endian Endian>
std::expected<T, std::error_code> to(std::span<const std::uint8_t> bytes, std::size_t* offset);

template<typename T, std::endian Endian>
std::expected<T, std::error_code> to(std::span<const std::uint8_t> bytes, std::size_t offset);

} // namespace bytes

} // namespace detail


//----------------[ Type Declarations ]----------------//

using serial_number_type = std::uint32_t;

namespace config
{

	enum class physical_unit_id;
	enum class enumeration_id;
	enum class sensor_id;

} // config

enum class sensor_value_rep_id : std::uint8_t
{
	empty = 0,
	registers = 1,
	integer = 2,
	physical = 3,
	enumeration = 4,
	COUNT = 5
};

struct sensor_value
{
	using empty = std::monostate;

	struct registers
	{
		static constexpr std::size_t max_size = 8;
		std::array<std::uint16_t, max_size> data{};
	};

	struct integer
	{
		std::int64_t value;
	};

	struct physical
	{
		double value;
		config::physical_unit_id unit_id;
	};

	struct enumeration
	{
		std::size_t index;
		config::enumeration_id enum_id;
	};

	constexpr sensor_value();
	constexpr sensor_value(registers value);
	constexpr sensor_value(integer value);
	constexpr sensor_value(physical value);
	constexpr sensor_value(enumeration value);

	[[nodiscard]] inline sensor_value_rep_id type() const;

	template<class T>
	[[nodiscard]] std::optional<T> get() const;

	template<class... F>
	auto visit(F&&... visitors);

	template<class... F>
	auto visit(F&&... visitors) const;

private:
	std::variant<
		empty,
		registers,
		integer,
		physical,
		enumeration
	> m_data;
};

struct sensor_value_rep
{
	struct registers
	{
		static constexpr auto max_size = sensor_value::registers::max_size;
	};

	struct integer
	{
		std::int32_t scale, offset;
	};

	struct physical
	{
		double scale, offset;
		config::physical_unit_id unit_id;
	};

	struct enumeration
	{
		config::enumeration_id enum_id;
	};

	constexpr sensor_value_rep(registers rep);
	constexpr sensor_value_rep(integer rep);
	constexpr sensor_value_rep(physical rep);
	constexpr sensor_value_rep(enumeration rep);

	[[nodiscard]] sensor_value_rep_id type() const;

	template<class T>
	[[nodiscard]] std::optional<T> get() const;

	[[nodiscard]] std::expected<sensor_value, std::error_code> interpret(std::span<const std::uint16_t> registers) const;

private:
	std::variant<
		registers,
		integer,
		physical,
		enumeration
	> m_data;
};

struct sensor_meta
{
	std::string_view name;
	std::uint16_t begin_address, register_count;
	sensor_value_rep rep;
};

struct physical_unit
{
	std::string_view measures, name, symbol;
};

struct enumeration
{
	std::span<const std::string_view> names;
};

[[nodiscard]] constexpr std::optional<sensor_meta> sensor_meta_by_id(config::sensor_id id);
[[nodiscard]] constexpr std::optional<physical_unit> physical_unit_by_id(config::physical_unit_id id);
[[nodiscard]] constexpr std::optional<enumeration> enumeration_by_id(config::enumeration_id id);


template<detail::tcp_socket Socket>
class connector
{
public:
	connector(serial_number_type serial_number);

	[[nodiscard]] std::error_code connect(const char* host, std::uint16_t port);

	[[nodiscard]] std::expected<sensor_value, std::error_code> read_sensor(config::sensor_id id);

	[[nodiscard]] std::error_code read_sensors(std::span<const config::sensor_id> sensor_ids, std::span<sensor_value> values);

	[[nodiscard]] std::error_code disconnect();

	[[nodiscard]] serial_number_type& serial_number();
	[[nodiscard]] const serial_number_type& serial_number() const;

protected:
	[[nodiscard]] std::expected<std::span<std::uint16_t>, std::error_code> read_registers(std::uint16_t begin_address, std::uint16_t register_count);

	[[nodiscard]] std::error_code write_registers(std::uint16_t begin_address, std::span<const std::uint16_t> values);

	template<class F, class G>
	[[nodiscard]] std::error_code modbus_request(std::size_t data_size, F&& write_request, G&& read_request);

	template<class F>
	[[nodiscard]] std::error_code send_modbus_frame(std::size_t data_size, F&& write_request);

	template<class F>
	[[nodiscard]] std::error_code receive_modbus_frame(F&& read_request);

private:
	Socket m_socket{};
	std::array<std::uint8_t, 2048> m_buffer{};
	serial_number_type m_serial_number{};
};
} // namespace deye


//====================[ implementations ]====================//

constexpr deye::sensor_value_rep::sensor_value_rep(registers rep) : m_data{ std::move(rep) } {}
constexpr deye::sensor_value_rep::sensor_value_rep(integer rep) : m_data{ std::move(rep) } {}
constexpr deye::sensor_value_rep::sensor_value_rep(physical rep) : m_data{ std::move(rep) } {}
constexpr deye::sensor_value_rep::sensor_value_rep(enumeration rep) : m_data{ std::move(rep) } {}

inline deye::sensor_value_rep_id deye::sensor_value_rep::type() const
{
	// To match up with the id enum the "empty" value of `sensor_value` is skipped.
	return static_cast<sensor_value_rep_id>(m_data.index() + 1);
}

template<class T>
inline std::optional<T> deye::sensor_value_rep::get() const
{
	if (const auto ptr = std::get_if<T>(&m_data); ptr != nullptr)
	{
		return *ptr;
	}
	return std::nullopt;
}

inline std::expected<deye::sensor_value, std::error_code> deye::sensor_value_rep::interpret(
	std::span<const std::uint16_t> raw_registers
) const {
	auto integer_value = std::uint64_t{};

	if (type() == sensor_value_rep_id::registers)
	{
		if (raw_registers.size() > registers::max_size)
		{
			return std::unexpected{ std::make_error_code(std::errc::result_out_of_range) };
		}
	}
	else
	{
		if (raw_registers.size_bytes() > sizeof(integer_value))
		{
			return std::unexpected{ std::make_error_code(std::errc::result_out_of_range) };
		}
		std::memcpy(&integer_value, raw_registers.data(), std::min(sizeof(integer_value), raw_registers.size()));
	}

	return std::visit(
		detail::overloaded_lambda{
			[&](const registers&) -> sensor_value
			{
				auto value = sensor_value::registers{};
				std::ranges::copy(raw_registers, value.data.begin());
				return { value };
			},
			[&](const integer& rep) -> sensor_value
			{
				auto value = static_cast<std::int64_t>(integer_value);
				value *= rep.scale;
				value += rep.offset;
				return {
					sensor_value::integer{
						.value = value
					}
				};
			},
			[&](const physical& rep) -> sensor_value
			{
				auto value = static_cast<double>(integer_value);
				value *= rep.scale;
				value += rep.offset;
				return {
					sensor_value::physical{
						.value = value,
						.unit_id = rep.unit_id
					}
				};
			},
			[&](const enumeration& rep) -> sensor_value
			{
				const auto index = integer_value;
				return {
					sensor_value::enumeration{
						.index = index,
						.enum_id = rep.enum_id
					}
				};
			},
		},
		m_data
	);
}

constexpr deye::sensor_value::sensor_value() : m_data{ empty{} } {}
constexpr deye::sensor_value::sensor_value(registers value) : m_data{ std::move(value) } {}
constexpr deye::sensor_value::sensor_value(integer value) : m_data{ std::move(value) } {}
constexpr deye::sensor_value::sensor_value(enumeration value) : m_data{ std::move(value) } {}
constexpr deye::sensor_value::sensor_value(physical value) : m_data{ std::move(value) } {}

deye::sensor_value_rep_id deye::sensor_value::type() const
{
	return static_cast<sensor_value_rep_id>(m_data.index());
}

template<class T>
std::optional<T> deye::sensor_value::get() const
{
	if (const auto ptr = std::get_if<T>(&m_data); ptr != nullptr)
	{
		return *ptr;
	}
	return std::nullopt;
}

template<class... F>
auto deye::sensor_value::visit(F&&... visitors)
{
	return std::visit(
		detail::overloaded_lambda{
			std::forward<F>(visitors)...
		},
		m_data
	);
}

template<class... F>
auto deye::sensor_value::visit(F&&... visitors) const
{
	return std::visit(
		detail::overloaded_lambda{
			std::forward<F>(visitors)...
		},
		m_data
	);
}

#include "deye_config.hpp"


//--------------[ connector implementation ]--------------//

namespace deye::connector_error
{
enum class codes
{
	ok = 0,
	action_exceeds_local_buffer_size,
	too_many_register_values,
	serial_number_mismatch,
	device_address_mismatch,
	unknown_response_code,
	response_invalid_start,
	response_invalid_end,
	response_wrong_checksum,
	response_wrong_crc,
	response_wrong_address,
	response_wrong_register_count,
	num_sensors_values_mismatch,
	unknown_sensor,
	unknown_unit,
	internal_error
};

struct category : std::error_category
{
	[[nodiscard]] const char* name() const noexcept override
	{
		return "deye_connector";
	}
	[[nodiscard]] std::string message(int ev) const override
	{
		switch (static_cast<codes>(ev))
		{
		case codes::action_exceeds_local_buffer_size:
			return "Action would exceed local buffer size.";
		case codes::too_many_register_values:
			return "The number of given values exceeds what can be written in one request.";
		case codes::device_address_mismatch:
			return "Device address does not match.";
		case codes::serial_number_mismatch:
			return "Serial Number does not match.";
		case codes::unknown_response_code:
			return "Unknown response error code.";
		case codes::response_invalid_start:
			return "Response frame has invalid starting byte";
		case codes::response_invalid_end:
			return "Response frame has invalid ending byte";
		case codes::response_wrong_checksum:
			return "Response frame checksum is not valid.";
		case codes::response_wrong_crc:
			return "Response frame crc is not valid.";
		case codes::response_wrong_address:
			return "Returned address does not match sent value.";
		case codes::response_wrong_register_count:
			return "Returned register count does not match sent value.";
		case codes::num_sensors_values_mismatch:
			return "Size of given value range does not match number of given sensor types.";
		case codes::unknown_sensor:
			return "Unknown sensor enum value.";
		case codes::unknown_unit:
			return "Unknown unit enum value.";
		case codes::internal_error:
			return "Internal error";
		default:
			return std::format("Device returned different serial number: {}", static_cast<serial_number_type>(ev));
		}
	}
};

} // namespace deye::connector_error

inline std::error_category& connector_error_category()
{
	static deye::connector_error::category category;
	return category;
}

namespace deye::connector_error
{
	inline std::error_code make_error_code(codes e)
	{
		return { static_cast<int>(e), connector_error_category() };
	}
} // namespace deye::connector_error


template <>
struct std::is_error_code_enum<deye::connector_error::codes> : std::true_type {};


constexpr std::optional<deye::sensor_meta> deye::sensor_meta_by_id(config::sensor_id id)
{
	if (const auto index = static_cast<std::size_t>(id); index < config::sensors.size())
	{
		return config::sensors[index];
	}
	else
	{
		return std::nullopt;
	}
}

constexpr std::optional<deye::physical_unit> deye::physical_unit_by_id(config::physical_unit_id id)
{
	const auto index = static_cast<std::size_t>(id);
	if (index < config::physical_units.size())
	{
		return config::physical_units[index];
	}
	else
	{
		return std::nullopt;
	}
}

constexpr std::optional<deye::enumeration> deye::enumeration_by_id(config::enumeration_id id)
{
	const auto index = static_cast<std::size_t>(id);
	if (index < config::enumerations.size())
	{
		return enumeration{ .names = config::enumerations[index] };
	}
	else
	{
		return std::nullopt;
	}
}


//--------------[ byte util implementation ]--------------//

constexpr std::uint8_t deye::detail::modbus::checksum(std::span<const std::uint8_t> data)
{
	return std::accumulate(data.begin(), data.end(), std::uint8_t{});
}

constexpr std::uint16_t deye::detail::modbus::crc(std::span<const std::uint8_t> data)
{
	std::uint16_t crc = 0xFFFF;

	for (const auto& byte : data)
	{
		crc ^= static_cast<std::uint16_t>(byte);

		for (int i = 0; i != 8; ++i)
		{
			if ((crc & 0x0001) != 0) {
				crc >>= 1;
				crc ^= 0xA001;
			} else {
				crc >>= 1;
			}
		}
	}

	return crc;
}


template<typename T, std::endian Endian>
std::error_code deye::detail::bytes::from(
	const auto& value, std::span<std::uint8_t> bytes,
	std::size_t* offset
) {
	if constexpr (is_container<decltype(value), const T>::value)
	{
		for (const auto &element : value)
		{
			if (const auto error = from<T, Endian>(element, bytes, offset))
			{
				return error;
			}
		}
	}
	else if (*offset >= bytes.size() or (bytes.size() - *offset) < sizeof(T))
	{
		return std::make_error_code(std::errc::result_out_of_range);
	}
	else
	{
		auto t_value = static_cast<T>(value);

		if constexpr (std::integral<T> and Endian != std::endian::native)
		{
			t_value = std::byteswap(t_value);
		}

		std::memcpy(&bytes[*offset], &t_value, sizeof(T));

		*offset += sizeof(T);
	}

	return {};
}

template<typename T, std::endian Endian>
std::error_code deye::detail::bytes::from(
	const auto& value,
	std::span<std::uint8_t> bytes,
	std::size_t offset
) {
	return from<T, Endian>(value, bytes, &offset);
}

template<typename T, std::endian Endian>
std::expected<T, std::error_code> deye::detail::bytes::to(
	std::span<const std::uint8_t> bytes,
	std::size_t* offset
) {
	if (*offset >= bytes.size() or (bytes.size() - *offset) < sizeof(T))
	{
		return std::unexpected{ std::make_error_code(std::errc::result_out_of_range) };
	}

	T value;

	std::memcpy(&value, &bytes[*offset], sizeof(T));

	if constexpr (std::integral<T> and Endian != std::endian::native)
	{
		value = std::byteswap(value);
	}

	*offset += sizeof(T);

	return value;
}

template<typename T, std::endian Endian>
std::expected<T, std::error_code> deye::detail::bytes::to(
	std::span<const std::uint8_t> bytes,
	std::size_t offset
) {
	return to<T, Endian>(bytes, &offset);
}

template<deye::detail::tcp_socket Socket>
deye::connector<Socket>::connector(serial_number_type serial_number) :
	m_serial_number{ serial_number } {}

template<deye::detail::tcp_socket Socket>
std::error_code deye::connector<Socket>::connect(const char* host, const std::uint16_t port)
{
	return m_socket.connect(host, port);
}

template<deye::detail::tcp_socket Socket>
std::error_code deye::connector<Socket>::disconnect()
{
	return m_socket.disconnect();
}

template<deye::detail::tcp_socket Socket>
deye::serial_number_type& deye::connector<Socket>::serial_number()
{
	return m_serial_number;
}

template<deye::detail::tcp_socket Socket>
const deye::serial_number_type& deye::connector<Socket>::serial_number() const
{
	return m_serial_number;
}

template<deye::detail::tcp_socket Socket>
template<class F>
std::error_code deye::connector<Socket>::send_modbus_frame(std::size_t data_size, F&& write_request)
{
	using connector_error::make_error_code;
	using connector_error::codes;

	const auto payload_size = (
		15			+			// data field
		data_size	+			// data
		sizeof(std::uint16_t) 	// crc
	);

	const auto frame_size = (
		sizeof(std::uint8_t)	+	// start byte
		sizeof(std::uint16_t)	+	// payload length
		sizeof(std::uint16_t)	+	// control code
		sizeof(std::uint16_t)	+	// inverter serial number prefix
		sizeof(serial_number_type)	+	// serial number
		payload_size 			+	// payload
		sizeof(std::uint8_t)	+	// checksum
		sizeof(std::uint8_t)		// end byte
	);

	if (frame_size > m_buffer.size())
	{
		return make_error_code(codes::action_exceeds_local_buffer_size);
	}

	auto frame = std::span{ m_buffer.data(), frame_size };

	auto offset = std::size_t{};

	namespace bytes = detail::bytes;

	if (std::error_code error;
		((error = bytes::from<std::uint8_t	    , std::endian::little>(0xa5			, frame, &offset))) or // start byte
		((error = bytes::from<std::uint16_t	    , std::endian::little>(payload_size		, frame, &offset))) or // payload size
		((error = bytes::from<std::uint16_t	    , std::endian::little>(0x4510		, frame, &offset))) or // control code
		((error = bytes::from<std::uint16_t		, std::endian::little>(0x0000		, frame, &offset))) or // inverter_sn_prefix
		((error = bytes::from<serial_number_type, std::endian::little>(m_serial_number	, frame, &offset))) or // serial number
		((error = bytes::from<std::uint8_t		, std::endian::little>(0x2			, frame, &offset))) or // data field
		((error = bytes::from<std::uint16_t		, std::endian::little>(0x00			, frame, &offset))) or // "
		((error = bytes::from<std::uint32_t		, std::endian::little>(0x0000		, frame, &offset))) or // "
		((error = bytes::from<std::uint64_t		, std::endian::little>(0x00000000	, frame, &offset))) 	// "
	) {
		return error;
	}

	auto data = frame.subspan(offset, data_size);
	if (const auto error = write_request(data))
	{
		return error;
	}

	offset += data_size;

	const auto crc = detail::modbus::crc(data);
	if (const auto error = bytes::from<std::uint16_t, std::endian::little>(crc, frame, &offset))
	{
		return error;
	}

	static constexpr auto ignore_start_byte = sizeof(std::uint8_t);
	const auto checksum = detail::modbus::checksum(frame.subspan(ignore_start_byte, offset - ignore_start_byte));

	if (std::error_code error;
	    ((error = bytes::from<std::uint8_t , std::endian::little>(checksum,	frame, &offset))) or // checksum placeholder
	    ((error = bytes::from<std::uint8_t , std::endian::little>(0x15,	frame, &offset)))	  // end byte
	) {
		return error;
	}


	return m_socket.send(frame);
}

template<deye::detail::tcp_socket Socket>
template<class F>
std::error_code deye::connector<Socket>::receive_modbus_frame(F&& read_request)
{
	using connector_error::make_error_code;
	using connector_error::codes;

	//-------------[ receive header ]-------------//

	constexpr auto header_size = (
		sizeof(std::uint8_t)  		+ // start byte
		sizeof(std::uint16_t) 		+ // data length
		sizeof(std::uint16_t) 		+ // control code
		sizeof(std::uint16_t) 		+ // inverter serial number prefix
		sizeof(serial_number_type)	  // serial number
	);

	static_assert(header_size < m_buffer.size());

	const auto header = std::span{ m_buffer.data(), header_size };

	if (const auto error = m_socket.receive(header))
	{
		return error;
	}

	//-------------[ check header ]-------------//

	if (header.front() != 0xa5)
	{
		return make_error_code(codes::response_invalid_start);
	}

	namespace bytes = detail::bytes;


	if (const auto returned_serial_number = bytes::to<serial_number_type, std::endian::little>(header, 7))
	{
		if (returned_serial_number.value() != m_serial_number)
		{
			// TODO this will lose precision on 32 bits and smaller machines.
			return { static_cast<int>(returned_serial_number.value()), connector_error_category() };
		}
	}

	const auto data_size = bytes::to<std::uint16_t, std::endian::little>(header, 1);
	if (not data_size)
	{
		return data_size.error();
	}

	//-------------[ receive body ]-------------//

	const auto body_size = (
		*data_size				+ // payload
		sizeof(std::uint8_t)	+ // checksum
		sizeof(std::uint8_t)	  // end byte
	);

	const auto full_size = header_size + body_size;
	if (full_size > m_buffer.size())
	{
		return make_error_code(codes::action_exceeds_local_buffer_size);
	}

	const auto message = std::span{ m_buffer.data(), full_size };
	auto body = message.subspan(header_size);

	if (const auto error = m_socket.receive(body))
	{
		return error;
	}

	//-------------[ check body ]-------------//

	 if (body.size() == 18)
	 {
	 	if (const auto code = bytes::to<std::uint16_t, std::endian::little>(body, 14))
	 	{
	 		codes errc;
	 		switch (code.value())
	 		{
	 		case 0x0005:
	 			errc = codes::device_address_mismatch;
	 			break;
	 		case 0x0006:
	 			errc = codes::serial_number_mismatch;
	 			break;
	 		default:
	 			errc = codes::unknown_response_code;
	 			break;
	 		}
	 		return make_error_code(errc);
	 	}
	 	else
	 	{
	 		return code.error();
	 	}
	}

	if (body.back() != 0x15)
	{
		return make_error_code(codes::response_invalid_end);
	}

	static constexpr auto ignore_end_byte = sizeof(std::uint8_t);
	body = body.subspan(0, body.size() - ignore_end_byte);

	static constexpr auto ignore_end_bytes = 2 * sizeof(std::uint8_t);

#ifdef DEYE_REDUNDANT_ERROR_CHECKS
	const auto expected_checksum = body.back();

	static constexpr auto ignore_start_byte = sizeof(std::uint8_t);
	const auto actual_checksum = detail::modbus::checksum(
		message.subspan(
			ignore_start_byte,
			message.size() - ignore_start_byte - ignore_end_bytes
		)
	);

	if (expected_checksum != actual_checksum)
	{
		return make_error_code(codes::response_wrong_checksum);
	}
#endif

	return read_request({ body.begin() + 14, body.end() - ignore_end_byte });
}


template<deye::detail::tcp_socket Socket>
template<class F, class G>
std::error_code deye::connector<Socket>::modbus_request(std::size_t data_size, F&& write_request, G&& read_request)
{
	if (const auto error = send_modbus_frame(data_size, std::forward<F>(write_request)))
	{
		return error;
	}

	if (const auto error = receive_modbus_frame(std::forward<G>(read_request)))
	{
		return error;
	}

	return {};
}

template<deye::detail::tcp_socket Socket>
std::expected<std::span<std::uint16_t>, std::error_code> deye::connector<Socket>::read_registers(
	const std::uint16_t begin_address,
	const std::uint16_t register_count
) {
	using connector_error::make_error_code;

	constexpr auto request_size = (
		sizeof(std::uint16_t) + // request type
		sizeof(std::uint16_t) + // begin address
		sizeof(std::uint16_t)   // register count
	);

	auto register_view = std::span<std::uint16_t>{};

	namespace bytes = detail::bytes;

	const auto write_request = [&](std::span<std::uint8_t> req) -> std::error_code
	{
		if (req.size() != request_size)
		{
			return make_error_code(connector_error::codes::internal_error);
		}

		auto offset = std::size_t{};
		if (std::error_code error;
			((error = bytes::from<std::uint16_t, std::endian::big>(0x0103	, req, &offset))) or
			((error = bytes::from<std::uint16_t, std::endian::big>(begin_address	, req, &offset))) or
			((error = bytes::from<std::uint16_t, std::endian::big>(register_count, req, &offset)))
		) {
			return error;
		}

		return {};
	};

	const auto read_request = [&](std::span<std::uint8_t> res) -> std::error_code
	{
		static constexpr auto ignore_crc_bytes = sizeof(std::uint16_t);
		const auto data = res.subspan(0, res.size() - ignore_crc_bytes);

#ifdef DEYE_REDUNDANT_ERROR_CHECKS
		const auto expected_crc = detail::modbus::crc(data);

		if (const auto actual_crc = bytes::to<std::uint16_t, std::endian::little>(res, data.size()))
		{
			if (actual_crc.value() != expected_crc)
			{
				return make_error_code(connector_error::codes::response_wrong_crc);
			}
		}
		else
		{
			return actual_crc.error();
		}
#endif

		const auto returned_register_byte_count = detail::bytes::to<std::uint8_t, std::endian::big>(data, 2);
		if (not returned_register_byte_count)
		{
			return returned_register_byte_count.error();
		}

		if (returned_register_byte_count.value() / sizeof(std::uint16_t) != register_count)
		{
			return make_error_code(connector_error::codes::response_wrong_register_count);
		}

		static constexpr auto register_offset = 3 * sizeof(std::uint8_t);

		if (data.size() < register_offset + register_count * sizeof(std::uint16_t))
		{
			return make_error_code(connector_error::codes::response_wrong_register_count);
		}

		register_view = {
			reinterpret_cast<std::uint16_t*>(data.data() + register_offset),
			register_count
		};

		return {};
	};

	if (const auto error = modbus_request(request_size, write_request, read_request))
	{
		return std::unexpected{ error };
	}

	if constexpr (std::endian::native != std::endian::big)
	{
		for (auto& reg : register_view)
		{
			reg = std::byteswap(reg);
		}
	}

	return register_view;
}

template<deye::detail::tcp_socket Socket>
std::error_code deye::connector<Socket>::write_registers(std::uint16_t begin_address, std::span<const std::uint16_t> values)
{
	using connector_error::make_error_code;
	using connector_error::codes;;

	if (values.size_bytes() > std::numeric_limits<std::uint8_t>::max())
	{
		return make_error_code(codes::too_many_register_values);
	}

	const auto request_size = (
		sizeof(std::uint16_t) +	// start bytes
		sizeof(std::uint16_t) +	// begin address
		sizeof(std::uint16_t) +	// address count
		sizeof(std::uint8_t) +	// byte count
		values.size_bytes()
	);

	namespace bytes = detail::bytes;

	const auto write_request = [&](std::span<std::uint8_t> req) -> std::error_code
	{
		if (req.size() != request_size)
		{
			return make_error_code(codes::internal_error);
		}

		auto offset = std::size_t{};
		if (
			std::error_code error;
			((error = bytes::from<std::uint16_t, std::endian::big>(0x1001			    , req, &offset))) or
			((error = bytes::from<std::uint16_t, std::endian::big>(begin_address	    		, req, &offset))) or
			((error = bytes::from<std::uint16_t, std::endian::big>(values.size()	    , req, &offset))) or
			((error = bytes::from<std::uint8_t , std::endian::big>(values.size_bytes()	, req, &offset))) or
			((error = bytes::from<std::uint16_t, std::endian::big>(values					, req, &offset)))
		) {
			return error;
		}

		return {};
	};

	const auto read_request = [&](std::span<std::uint8_t> res)
	{
#ifdef DEYE_REDUNDANT_ERROR_CHECKS
		static constexpr auto ignore_crc_bytes = sizeof(std::uint16_t);
		const auto data = res.subspan(0, res.size() - ignore_crc_bytes);
		const auto expected_crc = detail::modbus::crc(data);

		if (const auto actual_crc = bytes::to<std::uint16_t, std::endian::little>(res, data.size()))
		{
			if (actual_crc != expected_crc)
			{
				return make_error_code(codes::response_wrong_crc);
			}
		}
		else
		{
			return actual_crc.error();
		}
#endif

		if (const auto returned_address = bytes::to<std::uint16_t, std::endian::big>(res, 2))
		{
			if (returned_address.value() != begin_address)
			{
				return make_error_code(codes::response_wrong_address);
			}
		}
		else
		{
			return returned_address.error();
		}

		if (const auto returned_count = bytes::to<std::uint16_t, std::endian::big>(res, 4))
		{
			if (returned_count.value() != values.size())
			{
				return make_error_code(codes::response_wrong_address);
			}
		}
		else
		{
			return returned_count.error();
		}

		return {};
	};

	return modbus_request(request_size, write_request, read_request);
}

template<deye::detail::tcp_socket Socket>
[[nodiscard]] std::expected<deye::sensor_value, std::error_code> deye::connector<Socket>::read_sensor(
	const config::sensor_id id
) {
	using connector_error::make_error_code;

	if (const auto sensor_meta = sensor_meta_by_id(id))
	{
		if (const auto registers = read_registers(
			sensor_meta->begin_address, sensor_meta->register_count
		)) {
			if (const auto value = sensor_meta->rep.interpret(registers.value()))
			{
				return value.value();
			}
			else
			{
				return std::unexpected{ value.error() };
			}
		}
		else
		{
			return std::unexpected{ registers.error() };
		}
	}
	else
	{
		return std::unexpected{ make_error_code(connector_error::codes::unknown_sensor) };
	}
}

template<deye::detail::tcp_socket Socket>
std::error_code deye::connector<Socket>::read_sensors(
	std::span<const config::sensor_id> sensor_ids,
	std::span<sensor_value> sensor_values
) {
	using connector_error::make_error_code;

	if (sensor_ids.size() != sensor_values.size())
	{
		return make_error_code(connector_error::codes::num_sensors_values_mismatch);
	}

	if (sensor_ids.empty())
	{
		return {};
	}

	using address_limits = std::numeric_limits<std::uint16_t>;
	std::uint16_t begin_address{ address_limits::max() }, end_address{ address_limits::min() };

	for (const auto& sensor_id : sensor_ids)
	{
		if (const auto sensor = sensor_meta_by_id(sensor_id))
		{
			begin_address = std::min(begin_address, sensor->begin_address);
			end_address = std::max(end_address, static_cast<std::uint16_t>(sensor->begin_address + sensor->register_count));
		}
		else
		{
			return make_error_code(connector_error::codes::unknown_sensor);
		}
	}

	const auto register_count = end_address - begin_address;

	if (const auto registers = read_registers(begin_address, register_count))
	{
		for (auto [ sensor_id, sensor_value ] : std::views::zip(sensor_ids, sensor_values))
		{
			const auto sensor_meta = *sensor_meta_by_id(sensor_id);
			if (const auto value = sensor_meta.rep.interpret(
				registers->subspan(
					sensor_meta.begin_address - begin_address,
					sensor_meta.register_count
				)
			)) {
				sensor_value = value.value();
			}
			else
			{
				return value.error();
			}
		}
	}
	else
	{
		return registers.error();
	}

	return {};
}
