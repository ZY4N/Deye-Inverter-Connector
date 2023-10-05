#include <InfluxDBFactory.h>
#include <deye_connector.hpp>

#include <vector>
#include <thread>
#include <chrono>
#include "logger.hpp"


static constexpr char* ip = "INVERTER_IP";
static constexpr uint16_t port = 8899;
static constexpr uint32_t serial_number = INVERTER_SERIAL_NUMBER;
static constexpr char* influxdb_url = "http://localhost:8086?db=solar_inverters";


using namespace std::chrono_literals;

void logSensors(
	std::string_view ip,
	uint16_t port,
	uint32_t serial_number,
	std::span<const deye::sensor_types> sensors,
	influxdb::InfluxDB& db,
	std::string_view measurement_name
) {
   
	std::error_code e;
	
	deye::connector connector(serial_number);
	std::vector<double> values(sensors.size());
	
	const auto sleep = [&](const auto duration) {
		static constexpr auto TAG = "sleep";
		logger::debug("begin", measurement_name, TAG);
		
		using minute_t = std::chrono::duration<double, std::ratio<60>>;
		const auto minutes = std::chrono::duration_cast<minute_t>(duration);
		const auto msg = std::string("dt = ") + std::to_string(minutes.count()) + "min";
		
		logger::debug(msg, measurement_name, TAG);
		
		std::this_thread::sleep_for(duration);  
		
		logger::debug("end", measurement_name, TAG);
	};
	
	const auto connect = [&]() {
		static constexpr auto TAG = "connect";
		logger::debug("begin", measurement_name, TAG);
		while ((e = connector.connect(ip.data(), port))) {
			logger::error(connector.disconnect(), measurement_name, TAG);
			logger::error(e, measurement_name, TAG);
			sleep(10min);
		}
		logger::debug("end", measurement_name, TAG);
	};
	
	const auto read = [&]() {
		static constexpr auto TAG = "read";
		logger::debug("begin", measurement_name, TAG);
		while ((e = connector.read_sensors(sensors, values))) {
			logger::error(e, measurement_name, TAG);
			if (e.category() == boost::system::system_category()) {
			   return false;
			} else {
				sleep(10s);
			}
		}
		logger::debug("end", measurement_name, TAG);
		return true;
	};
	
	const auto store = [&]() {
		static constexpr auto TAG = "store";
		logger::debug("begin", measurement_name, TAG);
	
		auto dataPoint = influxdb::Point{ measurement_name.data() };
		for (size_t i = 0; i < sensors.size(); i++) {
			const auto my_sensor = deye::sensor::of(sensors[i]);
			if (my_sensor) {
				dataPoint.addField(my_sensor->name(), values[i]);
			}
		}
		
		try {
			db.write(std::move(dataPoint));
		} catch(const std::exception &e) {
			logger::warn(e.what(), measurement_name, TAG, "influx_error");
		} catch(...) {
			logger::warn("(unknown)", measurement_name, TAG, "influx_error");
		}
		
		logger::debug("end", measurement_name, TAG);
	};
	
	static constexpr auto TAG = "main";

	auto i = 0ULL;
	while (true) {
		
		connect();
		if (not read()) continue;
		logger::log(i++, measurement_name, TAG);
		store();
		sleep(10min);
		logger::error(connector.disconnect(), measurement_name,TAG);
	}
}


int main(int num_args, char* args[])  {
	
	logger::lvl = logger::level::DEBUG;
	
	if (num_args >= 2) {
		switch (args[1][0]) {
			case 'e': case 'E':
				logger::lvl = logger::level::ERROR;  break;
			case 'w': case 'W':
				logger::lvl = logger::level::WARN;  break;
			case 'i': case 'I':
				logger::lvl = logger::level::INFO;  break;
			case 'l': case 'L':
				logger::lvl = logger::level::LOG;  break;
			case 'd': case 'D':
				logger::lvl = logger::level::LOG;  break;
		}
	}
	
	logger::write(logger::level::ERROR, logger::level_name(logger::lvl), "log_level");


	const auto sensors = std::array{
		deye::sensor_types::PRODUCTION_TODAY,
		deye::sensor_types::PV1_VOLTAGE,
		deye::sensor_types::PV1_CURRENT,
		deye::sensor_types::RADIATOR_TEMPERATURE
	};
	
	auto db = influxdb::InfluxDBFactory::Get(influxdb_url);
	db->createDatabaseIfNotExists();
	
	logSensors(ip, port, serial_number, sensors, *db, "inverter1");
}
