# Deye Connector
This Header Only* C++20 library provides a simple interface to get data from your deye solar inverter like the .

*The library needs a socket_connection class to keep it platform independent. This way the provided Boost implementation can be replaced with another library for use on microcontrollers like the esp32.

```c++
#include <deye_connector.hpp>
#include <array>
#include <iostream>

static constexpr char* ip = "INVERTER_IP";
static constexpr uint16_t port = 8899;
static constexpr uint32_t serial_number = INVERTER_SERIAL_NUMBER;

int main() {
	constexpr auto my_sensors = std::array{
		deye::sensor_types::PRODUCTION_TODAY,
		deye::sensor_types::PV1_VOLTAGE,
		deye::sensor_types::PV1_CURRENT,
		deye::sensor_types::RADIATOR_TEMPERATURE
	};

	std::array<double, my_sensors.size()> values{};
	std::error_code error;

	deye::connector connector(serial_number);

	if ((error = connector.connect(ip, port))) {
		std::cout << "Error while connecting: " << error.message() << std::endl;
		return -1;
	}

	if ((error = connector.read_sensors(my_sensors, values))) {
		std::cout << "Error while reading: " << error.message() << std::endl;
		return -1;
	}

	for (size_t i = 0; i < values.size(); i++) {
		const auto my_sensor = deye::sensor::of(my_sensors[i]);
		std::cout <<
			my_sensor->name() << " (" << my_sensor->unit() << "): "
			<< values[i] << ' ' << my_sensor->symbol()
		<< std::endl;
	}
}
```
