#include <deye_connector.hpp>
#include <lwip_tcp_socket.hpp>
#include <wifi_client_handler.hpp>
#include <array>
#include <esp_log.h>


constexpr char wifi_ssid[] = "my_ssid";
constexpr char wifi_pwd[] = "my_password";
const uint32_t wifi_ip = wifi::parse_ip("192.168.3.3");
const uint32_t wifi_nm = wifi::parse_ip("0.0.255.255");
const uint32_t wifi_gw = wifi::parse_ip("192.168.1.1");
constexpr int wifi_retires = 10;

constexpr char ip[] = "1.1.1.1";
constexpr uint16_t port = 8899;
constexpr uint32_t serial_number = 123456;

static constexpr auto TAG = "main";

int main();

extern "C" {
int app_main() {
	return main();
}
}


int main() {
	constexpr auto my_sensors = std::array{
		deye::sensor_types::PRODUCTION_TODAY,
		deye::sensor_types::PV1_VOLTAGE,
		deye::sensor_types::PV1_CURRENT,
		deye::sensor_types::RADIATOR_TEMPERATURE
	};

	std::array<double, my_sensors.size()> values{};
	std::error_code error;
	wifi::client_handler wifi_client;
	
	if ((error = wifi_client.connect(
		wifi_ssid, wifi_pwd,
		wifi_ip, wifi_nm, wifi_gw,
		wifi_retires
	))) {
		ESP_LOGE(TAG, "Error while connecting to wifi: %s", error.message().c_str());
		return -1;
	}

	deye::connector<lwip_tcp_socket> connector(serial_number);

	if ((error = connector.connect(ip, port))) {
		ESP_LOGE(TAG, "Error while connecting: %s", error.message().c_str());
		return -1;
	}

	if ((error = connector.read_sensors(my_sensors, values))) {
		ESP_LOGE(TAG, "Error while reading: %s", error.message().c_str());
		return -1;
	}

	for (size_t i = 0; i < values.size(); i++) {
		const auto my_sensor = deye::sensor::of(my_sensors[i]);
		assert(my_sensor != nullptr);
		ESP_LOGE(TAG, "%s (%s): %lf %s",
			my_sensor->name().data(),
			my_sensor->unit().data(),
			values[i],
			my_sensor->symbol().data()
		);
	}
}
