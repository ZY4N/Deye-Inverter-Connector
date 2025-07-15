/*
* Copyright (C) 2025 ZY4N <me@zy4n.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

namespace deye::config
{

enum class physical_unit_id
{
	volts = 0,
	ampere = 1,
	watts = 2,
	watt_hours = 3,
	hertz = 4,
	degrees_celsius = 5,
	hours = 6,
	percentage = 7,
	COUNT = 8
};

enum class enumeration_id
{
	running_status = 0,
	gen_connected_status = 1,
	grid_status = 2,
	battery_status = 3,
	grid_connected_status = 4,
	smartload_enable_status = 5,
	work_mode = 6,
	time_of_use = 7,
	COUNT = 8
};

enum class sensor_id
{
	inverter_id = 0,
	control_board_version_num = 1,
	communication_board_version_num = 2,
	running_status = 3,
	total_grid_production = 4,
	daily_energy_bought = 5,
	daily_energy_sold = 6,
	total_energy_bought = 7,
	total_energy_sold = 8,
	daily_load_consumption = 9,
	total_load_consumption = 10,
	dc_temperature = 11,
	ac_temperature = 12,
	total_production = 13,
	alert = 14,
	daily_production = 15,
	pv1_voltage = 16,
	pv1_current = 17,
	pv2_voltage = 18,
	pv2_current = 19,
	grid_voltage_l1 = 20,
	grid_voltage_l2 = 21,
	load_voltage = 22,
	current_l1 = 23,
	current_l2 = 24,
	micro_inverter_power = 25,
	gen_connected_status = 26,
	gen_power = 27,
	internal_ct_l1_power = 28,
	internal_ct_l2_power = 29,
	grid_status = 30,
	total_grid_power = 31,
	external_ct_l1_power = 32,
	external_ct_l2_power = 33,
	inverter_l1_power = 34,
	inverter_l2_power = 35,
	total_power = 36,
	load_l1_power = 37,
	load_l2_power = 38,
	total_load_power = 39,
	battery_temperature = 40,
	battery_voltage = 41,
	battery_soc = 42,
	pv1_power = 43,
	pv2_power = 44,
	battery_status = 45,
	battery_power = 46,
	battery_current = 47,
	grid_connected_status = 48,
	smartload_enable_status = 49,
	work_mode = 50,
	time_of_use = 51,
	COUNT = 52
};

constexpr auto running_status_enum = std::array<std::string_view, 4>
{
	"Stand-by",
	"Self-checking",
	"Normal",
	"FAULT"
};

constexpr auto gen_connected_status_enum = std::array<std::string_view, 2>
{
	"OFF",
	"ON"
};

constexpr auto grid_status_enum = std::array<std::string_view, 3>
{
	"SELL",
	"BUY",
	"Stand-by"
};

constexpr auto battery_status_enum = std::array<std::string_view, 3>
{
	"Charge",
	"Stand-by",
	"Discharge"
};

constexpr auto grid_connected_status_enum = std::array<std::string_view, 2>
{
	"Off-Grid",
	"On-Grid"
};

constexpr auto smartload_enable_status_enum = std::array<std::string_view, 2>
{
	"OFF",
	"ON"
};

constexpr auto work_mode_enum = std::array<std::string_view, 5>
{
	"Selling First",
	"Zero-Export to Load&Solar Sell",
	"Zero-Export to Home&Solar Sell",
	"Zero-Export to Load",
	"Zero-Export to Home"
};

constexpr auto time_of_use_enum = std::array<std::string_view, 2>
{
	"Disable",
	"Enable"
};

constexpr auto enumerations = std::array<std::span<const std::string_view>, 8>
{
	std::span{ running_status_enum },
	std::span{ gen_connected_status_enum },
	std::span{ grid_status_enum },
	std::span{ battery_status_enum },
	std::span{ grid_connected_status_enum },
	std::span{ smartload_enable_status_enum },
	std::span{ work_mode_enum },
	std::span{ time_of_use_enum }
};

constexpr auto physical_units = std::array<physical_unit, 8>
{
	physical_unit{ "electric potential", "volts", "V" },
	physical_unit{ "current", "ampere", "A" },
	physical_unit{ "power", "watts", "W" },
	physical_unit{ "energy", "watt hours", "Wh" },
	physical_unit{ "frequency", "hertz", "Hz" },
	physical_unit{ "temperature", "Degrees Celsius", "°C" },
	physical_unit{ "time", "hours", "h" },
	physical_unit{ "fraction", "percentage", "%" }
};

constexpr auto sensors = std::array<sensor_meta, 52>
{
	sensor_meta{ "Inverter ID", 3, 5, { sensor_value_rep::integer{ 0, 0 } } },
	{ "Control Board Version No.", 13, 1, { sensor_value_rep::integer{ 0, 0 } } },
	{ "Communication Board Version No.", 14, 1, { sensor_value_rep::integer{ 0, 0 } } },
	{ "Running Status", 59, 1, { sensor_value_rep::enumeration{ enumeration_id::running_status } } },
	{ "Total Grid Production", 63, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Daily Energy Bought", 76, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Daily Energy Sold", 77, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Total Energy Bought", 78, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Total Energy Sold", 81, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Daily Load Consumption", 84, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Total Load Consumption", 85, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "DC Temperature", 90, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	{ "AC Temperature", 91, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	{ "Total Production", 96, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "Alert", 101, 6, { sensor_value_rep::integer{ 0, 0 } } },
	{ "Daily Production", 108, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	{ "PV1 Voltage", 109, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	{ "PV1 Current", 110, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	{ "PV2 Voltage", 111, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	{ "PV2 Current", 112, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	{ "Grid Voltage L1", 150, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	{ "Grid Voltage L2", 151, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	{ "Load Voltage", 157, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	{ "Current L1", 164, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	{ "Current L2", 165, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	{ "Micro-inverter Power", 166, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Gen-connected Status", 166, 1, { sensor_value_rep::enumeration{ enumeration_id::gen_connected_status } } },
	{ "Gen Power", 166, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Internal CT L1 Power", 167, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Internal CT L2 Power", 168, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Grid Status", 169, 1, { sensor_value_rep::enumeration{ enumeration_id::grid_status } } },
	{ "Total Grid Power", 169, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "External CT L1 Power", 170, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "External CT L2 Power", 171, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Inverter L1 Power", 173, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Inverter L2 Power", 174, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Total Power", 175, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Load L1 Power", 176, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Load L2 Power", 177, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Total Load Power", 178, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Battery Temperature", 182, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	{ "Battery Voltage", 183, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::volts } } },
	{ "Battery SOC", 184, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::percentage } } },
	{ "PV1 Power", 186, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "PV2 Power", 187, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Battery Status", 190, 1, { sensor_value_rep::enumeration{ enumeration_id::battery_status } } },
	{ "Battery Power", 190, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	{ "Battery Current", 191, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	{ "Grid-connected Status", 194, 1, { sensor_value_rep::enumeration{ enumeration_id::grid_connected_status } } },
	{ "SmartLoad Enable Status", 195, 1, { sensor_value_rep::enumeration{ enumeration_id::smartload_enable_status } } },
	{ "Work Mode", 244, 2, { sensor_value_rep::enumeration{ enumeration_id::work_mode } } },
	{ "Time of use", 248, 1, { sensor_value_rep::enumeration{ enumeration_id::time_of_use } } }
};

} // namespace deye::config
