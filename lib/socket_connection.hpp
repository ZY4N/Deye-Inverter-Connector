#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ts/internet.hpp>
#include <system_error>
#include <cstdint>
#include <span>

namespace asio = boost::asio;

class socket_connection {
public:
	socket_connection();

	[[nodiscard]] std::error_code listen(uint16_t port);
	
	[[nodiscard]] std::error_code connect(const std::string_view& host, uint16_t port);
	
	[[nodiscard]] std::error_code send(std::span<const uint8_t> data);
	
	[[nodiscard]] std::error_code receive(std::span<uint8_t> data);
	
	[[nodiscard]] std::error_code disconnect();

	~socket_connection();

private:
	asio::io_service ctx;
	asio::ip::tcp::socket socket;
};
