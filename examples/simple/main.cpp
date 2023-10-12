#include <deye_connector.hpp>
#include <asio_tcp_socket.hpp>
#include <array>
#include <iostream>

static constexpr char ip[] = "1.1.1.1";
static constexpr uint16_t port = 8899;
static constexpr uint32_t serial_number = 123456;

int main() {
	constexpr auto my_sensors = std::array{
		deye::sensor_types::PRODUCTION_TODAY,
		deye::sensor_types::PV1_VOLTAGE,
		deye::sensor_types::PV1_CURRENT,
		deye::sensor_types::RADIATOR_TEMPERATURE
	};

	std::array<double, my_sensors.size()> values{};
	std::error_code error;

	deye::connector<asio_tcp_socket> connector(serial_number);

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
		assert(my_sensor != nullptr);
		std::cout <<
			my_sensor->name() << " (" << my_sensor->unit() << "): "
			<< values[i] << ' ' << my_sensor->symbol()
		<< std::endl;
	}
}
