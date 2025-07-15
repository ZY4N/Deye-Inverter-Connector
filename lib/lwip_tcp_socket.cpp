/*
* Copyright (C) 2025 ZY4N <me@zy4n.com>
 *
 * Licensed under GPLv2, see file LICENSE in this source tree.
 */

#include "lwip_tcp_socket.hpp"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


static inline std::error_code make_system_error(int code) {
	using errc_t = std::underlying_type_t<std::errc>;
	const auto errc = static_cast<std::errc>(static_cast<errc_t>(code));
	return std::make_error_code(errc);
}


lwip_tcp_socket::lwip_tcp_socket(lwip_tcp_socket&& other) {
	std::swap(other.m_fd, m_fd);
}

lwip_tcp_socket& lwip_tcp_socket::operator=(lwip_tcp_socket&& other) {
	if (&other != this) {
		this->~lwip_tcp_socket();
		std::swap(other.m_fd, m_fd);
	}
	return *this;
}

std::error_code lwip_tcp_socket::listen(uint16_t port) {

	if (m_fd >= 0) {
		if (auto error = disconnect(); error) {
			return error;
		}
	}

	int listen_fd = -1;
	{
		if ((listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
			goto on_error;

		int reuse_address = true;
		if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_address, sizeof(reuse_address)) != 0)
			goto on_error;

		struct sockaddr_storage dest_addr;
		auto &dest_addr_ip4 = *(struct sockaddr_in *)&dest_addr;
		dest_addr_ip4.sin_addr.s_addr = htonl(INADDR_ANY);
		dest_addr_ip4.sin_family = AF_INET;
		dest_addr_ip4.sin_port = htons(port);

		if (bind(listen_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0)
			goto on_error;

		if (::listen(listen_fd, 1) != 0)
			goto on_error;
		
		struct sockaddr_storage source_addr;
		socklen_t addr_len = sizeof(source_addr);

		int conn_fd = ::accept(listen_fd, (struct sockaddr *)&source_addr, &addr_len);
		if (conn_fd < 0)
			goto on_error;
		
		m_fd = conn_fd;
	}

	return {};

on_error:
	if (listen_fd >= 0) {
		close(listen_fd);
	}

	return make_system_error(errno);
}

std::error_code lwip_tcp_socket::connect(const char* host, uint16_t port) {

	if (m_fd >= 0) {
		if (auto error = disconnect(); error) {
			return error;
		}
	}

	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);

	if (int ret = inet_pton(AF_INET, host, &dest_addr.sin_addr); ret < 1)
		return make_system_error(ret < 0 ? errno : EINVAL);

	int conn_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (conn_fd < 0)
		return make_system_error(errno);

	if (::connect(conn_fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != 0) {
		close(conn_fd);
		return make_system_error(errno);
	}

	m_fd = conn_fd;
	
	return {};
}

std::error_code lwip_tcp_socket::send(std::span<const uint8_t> bytes_left) {
	while (not bytes_left.empty()) {
		const auto sent = ::send(m_fd, bytes_left.data(), bytes_left.size(), 0);
		if (sent <= 0 and errno != EINTR)
			return make_system_error(errno);
		bytes_left = bytes_left.subspan(sent);
	}
	return {};
}

std::error_code lwip_tcp_socket::receive(std::span<uint8_t> bytes_left) {
	while (not bytes_left.empty()) {
		const auto received = recv(m_fd, bytes_left.data(), bytes_left.size(), 0);
		if (received <= 0 and errno != EINTR)
			return make_system_error(errno);
		bytes_left = bytes_left.subspan(received);
	}
	return {};
}

std::error_code lwip_tcp_socket::disconnect() {
	if (m_fd >= 0) {
		if (
			not shutdown(m_fd, SHUT_RDWR)
			and not close(m_fd)
		) {
			m_fd = -1;
		} else {
			return make_system_error(errno);
		}
	}
	return {};
}

lwip_tcp_socket::~lwip_tcp_socket() {
	disconnect();
}
