#include <socket_connection.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

socket_connection::socket_connection() :
	ctx{ }, socket{ ctx } {};

std::error_code socket_connection::connect(const std::string_view &host, const uint16_t port) {
    using namespace std::chrono_literals;
	boost::system::error_code error; 

	const auto ip = asio::ip::address::from_string(host.data(), error);
	if (error) return error;

	ctx.restart();
	socket.connect(tcp::endpoint(ip, port), error);
	
	return error;
}

std::error_code socket_connection::listen(const uint16_t port) {
	boost::system::error_code error;
	const auto endpoint = tcp::endpoint(tcp::v4(), port);
	return tcp::acceptor(ctx, endpoint).accept(socket, error);
}

std::error_code socket_connection::send(std::span<const uint8_t> data) {
	boost::system::error_code error;
	asio::write(socket, asio::const_buffer{ data.data(), data.size() }, error);
	return error;
}

std::error_code socket_connection::receive(std::span<uint8_t> data) {
	boost::system::error_code error;
	asio::read(socket, asio::mutable_buffer{ data.data(), data.size() }, error);
	return error;
}

std::error_code socket_connection::disconnect() {
    boost::system::error_code error;
    socket.shutdown(asio::ip::tcp::socket::shutdown_both, error);
    if (error) return error;
    socket.close();
    ctx.stop();
    return error;
}

socket_connection::~socket_connection() {
    [[maybe_unused]] const auto error = disconnect();
}
