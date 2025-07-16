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
		deye::config::sensor_id::inverter_id,
		deye::config::sensor_id::control_board_version_num,
		deye::config::sensor_id::communication_board_version_num,
		deye::config::sensor_id::running_status,
		deye::config::sensor_id::production_today,
		deye::config::sensor_id::uptime,
		deye::config::sensor_id::total_grid_production,
		deye::config::sensor_id::pv1_production_today,
		deye::config::sensor_id::pv2_production_today,
		deye::config::sensor_id::pv3_production_today,
		deye::config::sensor_id::pv4_production_today,
		deye::config::sensor_id::pv1_production_total,
		deye::config::sensor_id::pv2_production_total,
		deye::config::sensor_id::phase_1_voltage,
		deye::config::sensor_id::pv3_production_total,
		deye::config::sensor_id::daily_energy_bought,
		deye::config::sensor_id::phase_1_current,
		deye::config::sensor_id::daily_energy_sold,
		deye::config::sensor_id::pv4_production_total,
		deye::config::sensor_id::total_energy_bought,
		deye::config::sensor_id::ac_frequency,
		deye::config::sensor_id::operation_power,
		deye::config::sensor_id::total_energy_sold,
		deye::config::sensor_id::daily_load_consumption,
		deye::config::sensor_id::total_load_consumption,
		deye::config::sensor_id::ac_active_power,
		deye::config::sensor_id::dc_temperature,
		deye::config::sensor_id::ac_temperature,
		deye::config::sensor_id::total_production
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
				std::cout << std::hex << "[ ";
				for (const auto& reg : registers.data)
				{
					std::cout << "0x" << static_cast<int>(reg) << ' ';
				}
				std::cout << "]" << std::dec;
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
