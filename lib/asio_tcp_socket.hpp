#pragma once

#include <system_error>
#include <cstdint>
#include <span>

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ts/internet.hpp>

class asio_tcp_socket {
public:
	asio_tcp_socket();

	[[nodiscard]] std::error_code listen(uint16_t port);
	
	[[nodiscard]] std::error_code connect(const char* host, uint16_t port);

	[[nodiscard]] std::error_code send(std::span<const uint8_t> data);
	
	[[nodiscard]] std::error_code receive(std::span<uint8_t> data);
	
	[[nodiscard]] std::error_code disconnect();

	~asio_tcp_socket();

private:
	boost::asio::io_context ctx;
	boost::asio::ip::tcp::socket socket;
};
