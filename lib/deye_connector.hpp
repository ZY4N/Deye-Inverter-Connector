#pragma once

#include <socket_connection.hpp>

#include <system_error>
#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <span>

#include <algorithm>
#include <numeric>

// The deye modbus protocoll sends two checksums on top of the TCP connection.
// Since TCP has it's own error correction these checksums don't need to be checked
// but the checks can be enabled with the following define:
// #define DEYE_REDUNDANT_ERROR_CHECKS

namespace deye {

//====================[ interface ]====================//


//--------------[ connector interface ]--------------//

enum class sensor_types : uint8_t;

class connector {
public:
	connector(uint32_t n_serial_number);

	[[nodiscard]] inline std::error_code connect(const std::string_view& host, uint16_t port);

	[[nodiscard]] inline std::error_code read_sensor(sensor_types type, double &value);

	[[nodiscard]] inline std::error_code read_sensors(std::span<const sensor_types> types, std::span<double> values);

	[[nodiscard]] inline std::error_code disconnect();

	uint32_t &serial_number();
	const uint32_t &serial_number() const;

protected:
	[[nodiscard]] inline std::error_code read_registers(uint16_t begin_address, uint16_t register_count, std::span<uint16_t> &registers);

	[[nodiscard]] inline std::error_code write_registers(uint16_t begin_address, std::span<const uint8_t> values);

private:
	template<class F, class G>
	[[nodiscard]] inline std::error_code modbus_request(size_t dataSize, F&& writeRequest, G&& readResponse);

	template<class F>
	[[nodiscard]] inline std::error_code send_modbus_frame(size_t dataSize, F&& writeRequest);

	template<class F>
	[[nodiscard]] inline std::error_code receive_modbus_frame(F&& readResponse);

private:
	socket_connection m_conn{};
	std::vector<uint8_t> m_buffer{};
	uint32_t m_serial_number{};
};



//--------------[ sensor interface ]--------------//

enum class sensor_types : uint8_t {
	PRODUCTION_TODAY,
	PRODUCTION_TOTAL,
	PHASE1_VOLTAGE,
	PHASE1_CURRENT,
	AC_FREQUENCY,
	UPTIME,
	PV1_VOLTAGE,
	PV1_CURRENT,
	PV1_PRODUCTION_TODAY,
	PV1_PRODUCTION_TOTAL,
	PV2_VOLTAGE,
	PV2_CURRENT,
	PV2_PRODUCTION_TODAY,
	PV2_PRODUCTION_TOTAL,
	PV3_VOLTAGE,
	PV3_CURRENT,
	PV3_PRODUCTION_TODAY,
	PV3_PRODUCTION_TOTAL,
	PV4_VOLTAGE,
	PV4_CURRENT,
	PV4_PRODUCTION_TODAY,
	PV4_PRODUCTION_TOTAL,
	OPERATING_POWER,
	AC_ACTIVE_POWER,
	RADIATOR_TEMPERATURE
};


enum class physical_unit_types : uint8_t;

struct physical_unit {
	std::string_view measures{ "" }, name{ "" }, symbol{ "" };
	[[nodiscard]] inline constexpr static const physical_unit* of(physical_unit_types type);
};

struct scaled_physical_unit {
	template<class ratio>
	[[nodiscard]] constexpr inline static scaled_physical_unit from(physical_unit_types type);

	double scale;
	physical_unit_types type;
};


class sensor {
public:

	[[nodiscard]] constexpr inline static const sensor* of(sensor_types type);

	constexpr sensor() = default;

	constexpr sensor(
		std::string_view name,
		uint16_t begin_address,
		uint16_t num_registers,
		scaled_physical_unit unit,
		int32_t offset
	);

	[[nodiscard]] inline double read(std::span<const uint16_t> registers) const;

	[[nodiscard]] constexpr inline uint16_t begin_address() const;
	[[nodiscard]] constexpr inline uint16_t num_registers() const;

	[[nodiscard]] constexpr inline std::string_view name() const;
	[[nodiscard]] constexpr inline std::string_view measures() const;
	[[nodiscard]] constexpr inline std::string_view unit() const;
	[[nodiscard]] constexpr inline std::string_view symbol() const;

private:
	std::string_view m_name{ "" };
	uint16_t m_begin_address, m_num_registers;
	scaled_physical_unit m_unit;
	int32_t m_offset;
};


//====================[ implementations ]====================//


//--------------[ unit implementation ]--------------//


enum class physical_unit_types : uint8_t {
	VOLTS, AMPERE, WATTS, WATT_HOURS, HERTZ, DEGREES_CELCIUS, HOURS
};

inline constexpr auto all_physical_units = []() {
	std::array<physical_unit, 7> lookup{};
	const auto set = [&](physical_unit_types type, std::string_view measures, std::string_view name, std::string_view symbol) constexpr {
		const auto index = static_cast<uint8_t>(type);
		lookup[index] = { measures, name, symbol };
	};
	set(physical_unit_types::VOLTS				, "Voltage"		, "Volts"			, "V"	);
	set(physical_unit_types::AMPERE				, "Current"		, "Ampere"			, "A"	);
	set(physical_unit_types::WATTS				, "Power"		, "Watts"			, "W"	);
	set(physical_unit_types::WATT_HOURS			, "Energy"		, "Watt hours"		, "Wh"	);
	set(physical_unit_types::HERTZ				, "Frequency"	, "Hertz"			, "Hz"	);
	set(physical_unit_types::DEGREES_CELCIUS	, "Temperature"	, "degrees Ceclius"	, "C°"	);
	set(physical_unit_types::HOURS				, "Time"		, "hours"			, "h"	);
	return lookup;
}();

inline constexpr const physical_unit* physical_unit::of(physical_unit_types type) {
	const auto index = static_cast<uint8_t>(type);
	if (index < all_physical_units.size()) {
		return &all_physical_units[index];
	} else {
		return nullptr;
	}
}


template<class ratio>
[[nodiscard]] constexpr inline scaled_physical_unit scaled_physical_unit::from(physical_unit_types type) {
	return {
		.scale = double(ratio::num) / double(ratio::den),
		.type = type
	};
}


//--------------[ byte util implementation ]--------------//

namespace bytes {
	enum class endianness {
		BIG, LITTLE
	};

	static inline size_t ignoreNumBytes = 0;

	template<typename T, endianness Endian = endianness::BIG, class OutputIt>
	inline size_t from(const auto &value, OutputIt &dst);

	template<typename T, endianness Endian = endianness::BIG, class InputIt>
	inline T to(InputIt src, size_t &numBytes = ignoreNumBytes);
};


namespace bytes {
	namespace detail {
		template<typename T, typename U>
		struct is_container {
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
	}


	template<typename T, endianness Endian, class OutputIt>
	size_t from(const auto &value, OutputIt &dst) {
		size_t numBytes = 0;
		if constexpr (detail::is_container<decltype(value), const T>::value) {	
			for (const auto &element : value) {
				numBytes += from<T, Endian>(element, dst);
			}
		} else {
			const auto t_value = static_cast<T>(value);

			using bytes_t = const std::array<uint8_t, sizeof(T)>;
			const auto &bytes = *reinterpret_cast<bytes_t*>(&t_value);

			if constexpr (std::integral<T> and Endian == endianness::BIG) {	
				std::reverse_copy(bytes.begin(), bytes.end(), dst);
			} else {
				std::copy(bytes.begin(), bytes.end(), dst);
			}
			dst += numBytes = sizeof(T);
		}

		return numBytes;
	}


	template<typename T, endianness Endian, class InputIt>
	T to(InputIt src, size_t &numBytes) {
		T value;
		
		numBytes = sizeof(T);

		auto &dst = *reinterpret_cast<std::array<uint8_t, sizeof(T)>*>(&value);

		if constexpr (std::integral<T> and Endian == endianness::BIG) {
			std::reverse_copy(src, src + sizeof(T), dst.begin());
		} else {
			std::copy(src, src + sizeof(T), dst.begin());
		}

		return value;
	}
}


//--------------[ sensor implementation ]--------------//


constexpr sensor::sensor(
	std::string_view n_name,
	uint16_t n_begin_address,
	uint16_t n_num_registers,
	scaled_physical_unit n_unit,
	int32_t n_offset
) :
	m_name{ n_name },
	m_begin_address{ n_begin_address },
	m_num_registers{ n_num_registers },
	m_unit{ n_unit },
	m_offset{ n_offset } {}


constexpr inline uint16_t sensor::begin_address() const {
	return m_begin_address;
}

constexpr inline uint16_t sensor::num_registers() const {
	return m_num_registers;
}


inline double sensor::read(std::span<const uint16_t> registers) const {
	uint64_t value = 0;
	auto it = reinterpret_cast<uint8_t*>(&value);
	for (const auto &reg : registers) {
		bytes::from<uint16_t, bytes::endianness::BIG>(reg, it);
	}
	return m_unit.scale * static_cast<double>(static_cast<int32_t>(value) + m_offset);
}

[[nodiscard]] constexpr inline std::string_view sensor::name() const {
	return m_name;
}

[[nodiscard]] constexpr inline std::string_view sensor::measures() const {
	const auto &unit = *physical_unit::of(m_unit.type);
	return unit.measures;
}

[[nodiscard]] constexpr inline std::string_view sensor::unit() const {
	const auto &unit = *physical_unit::of(m_unit.type);
	return unit.name;
}

[[nodiscard]] constexpr inline std::string_view sensor::symbol() const {
	const auto &unit = *physical_unit::of(m_unit.type);
	return unit.symbol;
}


inline constexpr auto all_sensors = []() {
	std::array<sensor, 25> lookup{};
	const auto set = [&](sensor_types type, std::string_view name, uint16_t begin_address, uint16_t num_registers, scaled_physical_unit unit, int32_t offset = 0) constexpr {
		const auto index = static_cast<uint8_t>(type);
		lookup[index] = { name, begin_address, num_registers, unit, offset };
	};
	set(sensor_types::PRODUCTION_TODAY		, "Production Today"	, 60	, 1, scaled_physical_unit::from<std::hecto>(physical_unit_types::WATT_HOURS)	);
	set(sensor_types::PRODUCTION_TOTAL		, "Production total"	, 63	, 2, scaled_physical_unit::from<std::hecto>(physical_unit_types::WATT_HOURS)	);
	set(sensor_types::PHASE1_VOLTAGE		, "Phase 1 Voltage"		, 73	, 1, scaled_physical_unit::from<std::deci >(physical_unit_types::VOLTS) 		);
	set(sensor_types::PHASE1_CURRENT		, "Phase 1 Current"		, 76	, 1, scaled_physical_unit::from<std::deci >(physical_unit_types::AMPERE)		);
	set(sensor_types::AC_FREQUENCY			, "AC Frequency"		, 79	, 1, scaled_physical_unit::from<std::centi>(physical_unit_types::HERTZ)			);
	set(sensor_types::UPTIME				, "Uptime"				, 62	, 1, scaled_physical_unit::from<std::ratio<1,60>>(physical_unit_types::HOURS)	);
	set(sensor_types::PV1_VOLTAGE			, "PV1 Voltage"			, 109	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::VOLTS)			);
	set(sensor_types::PV1_CURRENT			, "PV1 Current"			, 110	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::AMPERE)			);
	set(sensor_types::PV1_PRODUCTION_TODAY	, "PV1 Production today", 65	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV1_PRODUCTION_TOTAL	, "PV1 Production total", 69	, 2, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV2_VOLTAGE			, "PV2 Voltage"			, 111	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::VOLTS)			);
	set(sensor_types::PV2_CURRENT			, "PV2 Current"			, 112	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::AMPERE)			);
	set(sensor_types::PV2_PRODUCTION_TODAY	, "PV2 Production today", 66	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV2_PRODUCTION_TOTAL	, "PV2 Production total", 71	, 2, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV3_VOLTAGE			, "PV3 Voltage"			, 113	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::VOLTS )			);
	set(sensor_types::PV3_CURRENT			, "PV3 Current"			, 114	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::AMPERE)			);
	set(sensor_types::PV3_PRODUCTION_TODAY	, "PV3 Production today", 67	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV3_PRODUCTION_TOTAL	, "PV3 Production total", 74	, 2, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV4_VOLTAGE			, "PV4 Voltage"			, 115	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::VOLTS)			);
	set(sensor_types::PV4_CURRENT			, "PV4 Current"			, 116	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::AMPERE)			);
	set(sensor_types::PV4_PRODUCTION_TODAY	, "PV4 Production today", 68	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::PV4_PRODUCTION_TOTAL	, "PV4 Production total", 77	, 2, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::OPERATING_POWER		, "Operation Power"		, 80	, 1, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::AC_ACTIVE_POWER		, "AC Active Power"		, 86	, 2, scaled_physical_unit::from<std::deci>(physical_unit_types::WATT_HOURS)		);
	set(sensor_types::RADIATOR_TEMPERATURE	, "Radiator Temperature", 90	, 1, scaled_physical_unit::from<std::centi>(physical_unit_types::DEGREES_CELCIUS), -1000 );
	return lookup;
}();

[[nodiscard]] constexpr inline const sensor* sensor::of(sensor_types type) {
	const auto index = static_cast<uint8_t>(type);
	if (index < all_sensors.size()) {
		return &all_sensors[index];
	} else {
		return nullptr;
	}
}


//--------------[ modbus error check implementation ]--------------//

namespace modbus {
	uint8_t checksum(std::span<const uint8_t> data) {
		return std::accumulate(data.begin(), data.end(), uint8_t(0));
	}

	uint16_t crc(std::span<const uint8_t> data) {
		uint16_t crc = 0xFFFF;
		
		for (const auto &b : data) {
			crc ^= (uint16_t)b;
		
			for (int i = 8; i != 0; i--) {
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
}



//--------------[ connector implementation ]--------------//

namespace connector_error {
	enum class codes {
		OK = 0,
		DEVICE_ADDRESS_MISMATCH,
		SERIAL_NUMBER_MISMATCH,
		UNKNOWN_RESPONSE_CODE,
		RESPONSE_INVALID_START,
		RESPONSE_INVALID_END,
		RESPONSE_WRONG_CRC,
		RESPONSE_WRONG_ADDRESS,
		RESPONSE_WRONG_REGISTER_COUNT,
		NUM_SENSORS_VALUES_MISMATCH,
		UNKNOWN_SENSOR,
		UNKNWON_UNIT,
		INTERNAL_ERROR
	};

	struct category : std::error_category {
		const char* name() const noexcept override {
			return "connector";
		}
		std::string message(int ev) const override {
			switch (static_cast<codes>(ev)) {
			case codes::DEVICE_ADDRESS_MISMATCH:
				return "Device address does not match.";
			case codes::SERIAL_NUMBER_MISMATCH:
				return "Serial Number does not match.";
			case codes::UNKNOWN_RESPONSE_CODE:
				return "Unknown response error code.";
			case codes::RESPONSE_INVALID_START:
				return "Response frame has invalid starting byte";
			case codes::RESPONSE_INVALID_END:
				return "Response frame has invalid ending byte";
			case codes::RESPONSE_WRONG_CRC:
				return "Response frame crc is not valid.";
			case codes::RESPONSE_WRONG_ADDRESS:
				return "Returned address does not match sent value.";
			case codes::RESPONSE_WRONG_REGISTER_COUNT:
				return "Returned register count does not match sent value.";
			case codes::NUM_SENSORS_VALUES_MISMATCH:
			    return "Size of given value range does not match number of given sensor types.";
			case codes::UNKNOWN_SENSOR:
			    return "Unknown sensor enum value.";
			case codes::UNKNWON_UNIT:
			    return "Unknown unit enum value.";
			case codes::INTERNAL_ERROR:
			    return "Internal error";
			default:
			    using namespace std::string_literals;
				return "unrecognized error ("s + std::to_string(ev) + ")";
			}
		}
	};
}


inline std::error_category &connector_error_category() {
	static connector_error::category category;
	return category;
}


namespace connector_error {
	inline std::error_code make_error_code(codes e) {
		return { static_cast<int>(e), connector_error_category() };
	}
}

}

// needs to be registered in base scope...
template <>
struct std::is_error_code_enum<deye::connector_error::codes> : public std::true_type {};

namespace deye {

connector::connector(uint32_t n_serial_number) :
	m_serial_number{ n_serial_number } {}


std::error_code connector::connect(const std::string_view &host, const uint16_t port) {
	return m_conn.connect(host, port);
}

std::error_code connector::disconnect() {
	return m_conn.disconnect();
}

uint32_t &connector::serial_number() {
	return m_serial_number;
}

const uint32_t &connector::serial_number() const {
	return m_serial_number;
}


template<class F>
std::error_code connector::send_modbus_frame(size_t dataSize, F&& writeRequest) {
	
	const auto payloadSize = (
		15			+		// data field
		dataSize	+		// data
		sizeof(uint16_t) 	// crc
	);
	
	const auto frameSize = (
		sizeof(uint8_t)		+	// start byte
		sizeof(uint16_t)	+	// payload length
		sizeof(uint16_t)	+	// control code
		sizeof(uint16_t)	+	// inverter serial number prefix
		sizeof(uint32_t)	+	// serial number
		payloadSize 		+	// payload
		sizeof(uint8_t)		+	// checksum
		sizeof(uint8_t)			// end byte 
	);

	m_buffer.resize(frameSize);

    {
		auto it = m_buffer.begin();
		
		using namespace bytes;

		from<uint8_t,  endianness::BIG   >(0xa5,			it); // start byte
		from<uint16_t, endianness::LITTLE>(payloadSize,		it); // payload size
		from<uint16_t, endianness::LITTLE>(0x4510,			it); // control code
		from<uint16_t, endianness::LITTLE>(0x0000,			it); // inverter_sn_prefix
		from<uint32_t, endianness::LITTLE>(m_serial_number,	it); // serial number
		from<uint8_t , endianness::LITTLE>(0x2,				it); // data field
		from<uint16_t, endianness::LITTLE>(0x00,			it); // "
		from<uint32_t, endianness::LITTLE>(0x0000,			it); // "
		from<uint64_t, endianness::LITTLE>(0x00000000,		it); // "

		auto data = std::span{ it, dataSize };
		if (const auto error = writeRequest(data); error) {
			return error;
		}
		it += dataSize;

		const auto crc = modbus::crc(data);
		from<uint16_t, endianness::LITTLE>(crc,				it); // crc

		const auto checksum = modbus::checksum({ m_buffer.begin() + sizeof(uint8_t), it });
		from<uint8_t , endianness::LITTLE>(checksum,		it); // checksum placeholder
		from<uint8_t , endianness::LITTLE>(0x15,			it); // end byte
	}


	return m_conn.send(m_buffer);
}


template<class F>
std::error_code connector::receive_modbus_frame(F&& readResponse) {
	
	using connector_error::make_error_code;

	std::error_code error;

	//-------------[ receive header ]-------------//
	
	constexpr auto headerSize = (
		sizeof(uint8_t)  + // start byte
		sizeof(uint16_t) + // data length
		sizeof(uint16_t) + // control code 
		sizeof(uint16_t) + // inverter serial number prefix
		sizeof(uint32_t)   // serial number
	);
	
	m_buffer.resize(headerSize);
	if ((error = m_conn.receive(m_buffer)))
		return error;


	//-------------[ check header ]-------------//

	if (m_buffer.front() != 0xA5)
		return make_error_code(connector_error::codes::RESPONSE_INVALID_START);


	const auto returnedSerialNumber = bytes::to<uint32_t, bytes::endianness::LITTLE>(&m_buffer[7]);
	if (returnedSerialNumber != m_serial_number) {
	    //std::cout << m_serial_number << ' ' << returnedSerialNumber << std::endl;
		return make_error_code(connector_error::codes::DEVICE_ADDRESS_MISMATCH);
	}

	const auto dataLength = bytes::to<uint16_t, bytes::endianness::LITTLE>(&m_buffer[1]);


	//-------------[ receive body ]-------------//

	const auto bodySize = (
		dataLength + // payload
		sizeof(uint8_t) + // checksum
		sizeof(uint8_t)   // end byte
	);

	m_buffer.resize(headerSize + bodySize);

	auto header = std::span{ m_buffer.begin(), headerSize };
	auto body   = std::span{ header.end()  , bodySize   };

	if ((error = m_conn.receive(body)))
		return error;


	//-------------[ check body ]-------------//

	 if (m_buffer.size() == 29) { // request error
		connector_error::codes errc;
		const auto code = bytes::to<uint16_t, bytes::endianness::LITTLE>(&m_buffer[25]);
		switch (code) {
			case 0x0005:	errc = connector_error::codes::DEVICE_ADDRESS_MISMATCH;	break;
			case 0x0006:	errc = connector_error::codes::SERIAL_NUMBER_MISMATCH;	break;
			default:		errc = connector_error::codes::UNKNOWN_RESPONSE_CODE;
		}
		return make_error_code(errc);
	}

	if (m_buffer.back() != 0x15)
		return make_error_code(connector_error::codes::RESPONSE_INVALID_END);

#ifdef DEYE_REDUNDANT_ERROR_CHECKS
	const auto expected_checksum = m_buffer[m_buffer.size() - 2];
	const auto actual_checksum = std::accumulate(m_buffer.begin() + 1, m_buffer.end() - 2, uint8_t(0));

	if (expected_checksum != actual_checksum)
		return make_error_code(connector_error::codes::RESPONSE_WRONG_CRC);
#endif

	return readResponse({ m_buffer.begin() + 25, m_buffer.end() - 2 });
}


template<class F, class G>
std::error_code connector::modbus_request(size_t dataSize, F&& writeRequest, G&& readResponse) {
	std::error_code error;
	(
		(error = send_modbus_frame(dataSize, std::forward<F>(writeRequest))) or
		(error = receive_modbus_frame(std::forward<G>(readResponse)))
	);
	return error;
}

std::error_code connector::read_registers(uint16_t begin_address, uint16_t register_count, std::span<uint16_t> &registers) {

	using connector_error::make_error_code;

	const auto requestSize = 3 * sizeof(uint16_t);

	const auto writeRequest = [&](std::span<uint8_t> req) {

		if (req.size() != requestSize)
			return make_error_code(connector_error::codes::INTERNAL_ERROR);

		auto it = req.data();
		bytes::from<uint16_t>(0x0103, it);
		bytes::from<uint16_t>(begin_address, it);
		bytes::from<uint16_t>(register_count, it);

		return make_error_code(connector_error::codes::OK);
	};

	const auto readResponse = [&](std::span<uint8_t> res) {
	    
		const auto data = std::span{ res.begin(), res.end() - sizeof(uint16_t) }; // crc is not part of data

#ifdef DEYE_REDUNDANT_ERROR_CHECKS
		const auto expected_crc = modbus::crc(data);
		const auto actual_crc = bytes::to<uint16_t, bytes::endianness::LITTLE>(data.end());
		
		if (actual_crc != expected_crc)
			return make_error_code(connector_error::codes::RESPONSE_WRONG_CRC);
#endif
		

		const auto reg_count_byte = data[2] / sizeof(uint16_t);
		
		if (reg_count_byte < register_count)
			return make_error_code(connector_error::codes::RESPONSE_WRONG_REGISTER_COUNT);

		auto registerBegin = reinterpret_cast<uint16_t*>(data.data() + 3);
		registers = { registerBegin, register_count };

		return make_error_code(connector_error::codes::OK);
	};


	return modbus_request(requestSize, writeRequest, readResponse);
}


std::error_code connector::write_registers(uint16_t begin_address, std::span<const uint8_t> values) {

	using connector_error::make_error_code;

	const auto requestSize = 7 + values.size() * sizeof(uint16_t);

	const auto writeRequest = [&](std::span<uint8_t> req) -> std::error_code {

		if (req.size() != requestSize)
			return make_error_code(connector_error::codes::INTERNAL_ERROR);

		auto it = req.data();
		bytes::from<uint16_t>(0x1001, it);
		bytes::from<uint16_t>(begin_address, it);
		bytes::from<uint16_t>(values.size(), it);
		bytes::from<uint8_t>(values.size() * sizeof(uint16_t), it);
		bytes::from<uint16_t>(values, it);
		
		return make_error_code(connector_error::codes::OK);
	};

	const auto readResponse = [&](std::span<uint8_t> res) {

#ifdef DEYE_REDUNDANT_ERROR_CHECKS
		const auto crcBegin = res.end() - sizeof(uint16_t);
		const auto expected_crc = modbus::crc(std::span{ res.begin(), crcBegin });
		const auto actual_crc = bytes::to<uint16_t, bytes::endianness::LITTLE>(crcBegin);
		
        if (actual_crc != expected_crc)
			return make_error_code(connector_error::codes::RESPONSE_WRONG_CRC);
#endif

        const auto returned_address = bytes::to<uint16_t, bytes::endianness::BIG>(&res[2]);
		const auto returned_count = bytes::to<uint16_t, bytes::endianness::BIG>(&res[4]);

        if (returned_address != begin_address)
			return make_error_code(connector_error::codes::RESPONSE_WRONG_ADDRESS);

        if (returned_count != values.size())
            return make_error_code(connector_error::codes::RESPONSE_WRONG_REGISTER_COUNT);

		return make_error_code(connector_error::codes::OK);
	};


	return modbus_request(requestSize, writeRequest, readResponse);
}


[[nodiscard]] std::error_code connector::read_sensor(const sensor_types type, double &value) {

	using connector_error::make_error_code;

	std::error_code error;

	const auto sensor = sensor::of(type);
	if (not sensor)
		return make_error_code(connector_error::codes::UNKNOWN_SENSOR);


	std::span<uint16_t> registers;
	if ((error = read_registers(sensor->begin_address(), sensor->num_registers(), registers)))
		return error;

	value = sensor->read(registers);

	return make_error_code(connector_error::codes::OK);
}


std::error_code connector::read_sensors(std::span<const sensor_types> types, std::span<double> values) {

	using connector_error::make_error_code;

	if (types.size() != values.size())
		return make_error_code(connector_error::codes::NUM_SENSORS_VALUES_MISMATCH);

	using address_limits = std::numeric_limits<uint16_t>;
	uint16_t begin_address{ address_limits::max() }, end_address{ address_limits::min() };

	for (const auto &type : types) {
		const auto sensor = sensor::of(type);
		if (not sensor)
			return make_error_code(connector_error::codes::UNKNOWN_SENSOR);

		begin_address = std::min(begin_address, sensor->begin_address());
		end_address = std::max(end_address, uint16_t(sensor->begin_address() + sensor->num_registers()));
	}

	const auto num_registers = end_address - begin_address;

	std::span<uint16_t> registers;
	if (const auto error = read_registers(begin_address, num_registers, registers); error)
		return error;
		
    const auto type_size = (ssize_t)types.size();
	for (ssize_t i = 0; i < type_size; i++) {
		const auto sensor = *sensor::of(types[i]);
		const auto registerIndex = sensor.begin_address() - begin_address;
		values[i] = sensor.read({
			registers.begin() + registerIndex,
			sensor.num_registers()
		});
	}

	return make_error_code(connector_error::codes::OK);
}
}
