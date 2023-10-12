#pragma once

#include <system_error>
#include <cstdint>
#include <span>


class lwip_tcp_socket {
public:
	lwip_tcp_socket() = default;

	lwip_tcp_socket(lwip_tcp_socket&& other);
	lwip_tcp_socket& operator=(lwip_tcp_socket&& other);

	lwip_tcp_socket(const lwip_tcp_socket& other) = delete;
	lwip_tcp_socket& operator=(const lwip_tcp_socket& other) = delete;

	[[nodiscard]] std::error_code listen(uint16_t port);
	
	[[nodiscard]] std::error_code connect(const char* host, uint16_t port);

	[[nodiscard]] std::error_code send(std::span<const uint8_t> data);

	[[nodiscard]] std::error_code receive(std::span<uint8_t> data);

	[[nodiscard]] std::error_code disconnect();

	~lwip_tcp_socket();	

private:
	int m_fd{ -1 };
};
