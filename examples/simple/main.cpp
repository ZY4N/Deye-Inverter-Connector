/*
 * Copyright (C) 2025 ZY4N <me@zy4n.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include <deye_connector.hpp>
#include <asio_tcp_socket.hpp>
#include <array>
#include <iostream>

static constexpr char ip[] = "192.168.2.22";
static constexpr uint16_t port = 8899;
static constexpr uint32_t serial_number = 4161840362;


template<class... Ts>
struct overloaded : Ts...
{
	using Ts::operator()...;
};


int main()
{
	constexpr auto my_sensors = std::array{
		deye::config::sensor_id::daily_production,
		deye::config::sensor_id::pv1_voltage,
		deye::config::sensor_id::pv1_current,
		deye::config::sensor_id::dc_temperature
	};

	std::array<deye::sensor_value, my_sensors.size()> values{};

	deye::connector<asio_tcp_socket> connector(serial_number);

	if (const auto error = connector.connect(ip, port))
	{
		std::cerr << "Error while connecting: " << error.message() << std::endl;
		return EXIT_FAILURE;
	}

	if (const auto error = connector.read_sensors(my_sensors, values))
	{
		std::cerr << "Error while reading: " << error.message() << std::endl;
		return EXIT_FAILURE;
	}

	for (const auto [ sensor_id, sensor_value ] : std::views::zip(my_sensors, values))
	{
		const auto sensor_meta = *deye::sensor_meta_by_id(sensor_id);

		std::cout << sensor_meta.name << ": ";

		sensor_value.visit(
			[&](const deye::sensor_value::integer& integer)
			{
				std::cout << integer.value << std::endl;
			},
			[&](const deye::sensor_value::physical& physical)
			{
				std::cout << physical.value << " " << deye::physical_unit_by_id(physical.unit_id)->symbol << std::endl;
			},
			[&](const deye::sensor_value::enumeration& enumeration)
			{
				std::cout << deye::enumeration_by_id(enumeration.enum_id)->names[enumeration.index];
			},
			[&](const deye::sensor_value::empty&)
			{
				std::cout << "<empty>";
			}
		);

		std::cout << std::endl;
	}

	return EXIT_SUCCESS;
}
