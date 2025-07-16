/*
 * Copyright (C) 2025 ZY4N <me@zy4n.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include <deye_connector.hpp>
#include <asio_tcp_socket.hpp>
#include <iostream>

static constexpr char ip[] = "1.1.1.1";
static constexpr uint16_t port = 8899;
static constexpr uint32_t serial_number = 69420;

int main()
{
	using enum deye::config::sensor_id;

	constexpr auto my_sensors = std::array{
		inverter_id, control_board_version_num, communication_board_version_num,
		running_status, production_today, uptime,
		total_grid_production, pv1_production_today, pv2_production_today,
		pv3_production_today, pv4_production_today, pv1_production_total,
		pv2_production_total, phase_1_voltage, pv3_production_total,
		daily_energy_bought, phase_1_current, daily_energy_sold,
		pv4_production_total, total_energy_bought, ac_frequency,
		operation_power, total_energy_sold, daily_load_consumption,
		total_load_consumption, ac_active_power, dc_temperature,
		ac_temperature, total_production
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
			[&](const deye::sensor_value::registers& registers)
			{
				const auto register_view = std::span{
					registers.data.data(),
					sensor_meta.register_count
				};
				if (sensor_id == deye::config::sensor_id::inverter_id)
				{
					std::cout << '"';
					for (const auto& reg : register_view)
					{
						std::cout << static_cast<char>(reg & 0xff);
						std::cout << static_cast<char>((reg >> 8) & 0xff);
					}
					std::cout << '"';
				}
				else
				{
					std::cout << std::hex << "[ ";
					for (const auto& reg : register_view)
					{
						std::cout << "0x" << static_cast<int>(reg) << ' ';
					}
					std::cout << "]" << std::dec;
				}
			},
			[&](const deye::sensor_value::integer& integer)
			{
				std::cout << integer.value;
			},
			[&](const deye::sensor_value::physical& physical)
			{
				std::cout << physical.value << " " << deye::physical_unit_by_id(physical.unit_id)->symbol;
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
