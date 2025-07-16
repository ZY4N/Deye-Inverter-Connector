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
	production_today = 4,
	uptime = 5,
	total_grid_production = 6,
	pv1_production_today = 7,
	pv2_production_today = 8,
	pv3_production_today = 9,
	pv4_production_today = 10,
	pv1_production_total = 11,
	pv2_production_total = 12,
	phase_1_voltage = 13,
	pv3_production_total = 14,
	daily_energy_bought = 15,
	phase_1_current = 16,
	daily_energy_sold = 17,
	pv4_production_total = 18,
	total_energy_bought = 19,
	ac_frequency = 20,
	operation_power = 21,
	total_energy_sold = 22,
	daily_load_consumption = 23,
	total_load_consumption = 24,
	ac_active_power = 25,
	dc_temperature = 26,
	ac_temperature = 27,
	total_production = 28,
	alert = 29,
	daily_production = 30,
	pv1_voltage = 31,
	pv1_current = 32,
	pv2_voltage = 33,
	pv2_current = 34,
	pv3_voltage = 35,
	pv3_current = 36,
	pv4_voltage = 37,
	pv4_current = 38,
	grid_voltage_l1 = 39,
	grid_voltage_l2 = 40,
	load_voltage = 41,
	current_l1 = 42,
	current_l2 = 43,
	micro_inverter_power = 44,
	gen_connected_status = 45,
	gen_power = 46,
	internal_ct_l1_power = 47,
	internal_ct_l2_power = 48,
	grid_status = 49,
	total_grid_power = 50,
	external_ct_l1_power = 51,
	external_ct_l2_power = 52,
	inverter_l1_power = 53,
	inverter_l2_power = 54,
	total_power = 55,
	load_l1_power = 56,
	load_l2_power = 57,
	total_load_power = 58,
	battery_temperature = 59,
	battery_voltage = 60,
	battery_soc = 61,
	pv1_power = 62,
	pv2_power = 63,
	battery_status = 64,
	battery_power = 65,
	battery_current = 66,
	grid_connected_status = 67,
	smartload_enable_status = 68,
	work_mode = 69,
	time_of_use = 70,
	COUNT = 71
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
constexpr auto sensors = std::array<sensor_meta, 71>
{
	sensor_meta{ "Inverter ID", 3, 5, { sensor_value_rep::registers{} } },
	sensor_meta{ "Control Board Version No.", 13, 1, { sensor_value_rep::integer{ 0, 0 } } },
	sensor_meta{ "Communication Board Version No.", 14, 1, { sensor_value_rep::integer{ 0, 0 } } },
	sensor_meta{ "Running Status", 59, 1, { sensor_value_rep::enumeration{ enumeration_id::running_status } } },
	sensor_meta{ "Production Today", 60, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Uptime", 62, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::hours } } },
	sensor_meta{ "Total Grid Production", 63, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV1 Production today", 65, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV2 Production today", 66, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV3 Production today", 67, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV4 Production today", 68, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV1 Production total", 69, 2, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV2 Production total", 71, 2, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Phase 1 Voltage", 73, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "PV3 Production total", 74, 2, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Daily Energy Bought", 76, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Phase 1 Current", 76, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	sensor_meta{ "Daily Energy Sold", 77, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV4 Production total", 77, 2, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Total Energy Bought", 78, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "AC Frequency", 79, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::hertz } } },
	sensor_meta{ "Operation Power", 80, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Total Energy Sold", 81, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Daily Load Consumption", 84, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Total Load Consumption", 85, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "AC Active Power", 86, 2, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "DC Temperature", 90, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	sensor_meta{ "AC Temperature", 91, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	sensor_meta{ "Total Production", 96, 2, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "Alert", 101, 6, { sensor_value_rep::registers{} } },
	sensor_meta{ "Daily Production", 108, 1, { sensor_value_rep::physical{ 100, 0, physical_unit_id::watt_hours } } },
	sensor_meta{ "PV1 Voltage", 109, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "PV1 Current", 110, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	sensor_meta{ "PV2 Voltage", 111, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "PV2 Current", 112, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	sensor_meta{ "PV3 Voltage", 113, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "PV3 Current", 114, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	sensor_meta{ "PV4 Voltage", 115, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "PV4 Current", 116, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::ampere } } },
	sensor_meta{ "Grid Voltage L1", 150, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "Grid Voltage L2", 151, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "Load Voltage", 157, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::volts } } },
	sensor_meta{ "Current L1", 164, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	sensor_meta{ "Current L2", 165, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	sensor_meta{ "Micro-inverter Power", 166, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Gen-connected Status", 166, 1, { sensor_value_rep::enumeration{ enumeration_id::gen_connected_status } } },
	sensor_meta{ "Gen Power", 166, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Internal CT L1 Power", 167, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Internal CT L2 Power", 168, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Grid Status", 169, 1, { sensor_value_rep::enumeration{ enumeration_id::grid_status } } },
	sensor_meta{ "Total Grid Power", 169, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "External CT L1 Power", 170, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "External CT L2 Power", 171, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Inverter L1 Power", 173, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Inverter L2 Power", 174, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Total Power", 175, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Load L1 Power", 176, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Load L2 Power", 177, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Total Load Power", 178, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Battery Temperature", 182, 1, { sensor_value_rep::physical{ 0.1, 0, physical_unit_id::degrees_celsius } } },
	sensor_meta{ "Battery Voltage", 183, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::volts } } },
	sensor_meta{ "Battery SOC", 184, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::percentage } } },
	sensor_meta{ "PV1 Power", 186, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "PV2 Power", 187, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Battery Status", 190, 1, { sensor_value_rep::enumeration{ enumeration_id::battery_status } } },
	sensor_meta{ "Battery Power", 190, 1, { sensor_value_rep::physical{ 1, 0, physical_unit_id::watts } } },
	sensor_meta{ "Battery Current", 191, 1, { sensor_value_rep::physical{ 0.01, 0, physical_unit_id::ampere } } },
	sensor_meta{ "Grid-connected Status", 194, 1, { sensor_value_rep::enumeration{ enumeration_id::grid_connected_status } } },
	sensor_meta{ "SmartLoad Enable Status", 195, 1, { sensor_value_rep::enumeration{ enumeration_id::smartload_enable_status } } },
	sensor_meta{ "Work Mode", 244, 2, { sensor_value_rep::enumeration{ enumeration_id::work_mode } } },
	sensor_meta{ "Time of use", 248, 1, { sensor_value_rep::enumeration{ enumeration_id::time_of_use } } }
};
} // namespace deye::config
