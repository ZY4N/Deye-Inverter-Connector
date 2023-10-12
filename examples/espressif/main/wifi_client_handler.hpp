#pragma once

#include <string_view>
#include <atomic>

#include "esp_error.hpp"
#include "esp_wifi.h"
#include <netdb.h>


namespace wifi {

	uint32_t parse_ip(const char *ip);

	class client_handler {

	public:
		client_handler() = default;

		client_handler(const client_handler&) = delete;
		client_handler& operator=(const client_handler& other) = delete;

		client_handler(client_handler&& other);
		client_handler& operator=(client_handler&& other);

		[[nodiscard]] std::error_code connect(
			const std::string_view& ssid, const std::string_view& password,
			uint32_t ip, uint32_t netmask, uint32_t gateway,
			uint32_t num_retries, uint32_t timeout_ms = 0
		);

		void disconnect();

		~client_handler();

	private:
		[[nodiscard]] int generic_init();

		struct connection_state {
			uint32_t ip, netmask, gateway;
			uint32_t current_retries, max_retries;
			esp_netif_t *handle{ nullptr };
			std::atomic_flag &connected;

			void event_handler(
				esp_event_base_t event_base,
				int32_t event_id,
				void* event_data
			);
		};

		inline static constexpr auto TAG = "WIFI_CLIENT";

		esp_netif_t *m_handle{ nullptr };
	};
}
