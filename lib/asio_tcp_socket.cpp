#include "asio_tcp_socket.hpp"

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

asio_tcp_socket::asio_tcp_socket() :
	ctx{ }, socket{ ctx } {};

std::error_code asio_tcp_socket::connect(const char* host, const uint16_t port) {
	using namespace std::chrono_literals;
	boost::system::error_code error;

	if (not ctx.stopped() and (error = disconnect())) {
		return error;
	}

	const auto ip = asio::ip::address::from_string(host, error);
	if (error) return error;

	ctx.restart();
	socket.connect(tcp::endpoint(ip, port), error);
	
	return error;
}

std::error_code asio_tcp_socket::listen(const uint16_t port) {
	boost::system::error_code error;

	if (not ctx.stopped() and (error = disconnect())) {
		return error;
	}

	const auto endpoint = tcp::endpoint(tcp::v4(), port);
	return tcp::acceptor(ctx, endpoint).accept(socket, error);
}

std::error_code asio_tcp_socket::send(std::span<const uint8_t> data) {
	boost::system::error_code error;
	asio::write(socket, asio::const_buffer{ data.data(), data.size() }, error);
	return error;
}

std::error_code asio_tcp_socket::receive(std::span<uint8_t> data) {
	boost::system::error_code error;
	asio::read(socket, asio::mutable_buffer{ data.data(), data.size() }, error);
	return error;
}

std::error_code asio_tcp_socket::disconnect() {
	boost::system::error_code error;
	socket.shutdown(asio::ip::tcp::socket::shutdown_both, error);
	if (error) return error;
	socket.close();
	ctx.stop();
	return error;
}

asio_tcp_socket::~asio_tcp_socket() {
	[[maybe_unused]] const auto error = disconnect();
}
