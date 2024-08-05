#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>

namespace beast_websocket
{
struct websocket_server
{
	websocket_server(websocket_server&&) = delete;

	websocket_server(boost::asio::io_context& ctx, uint16_t port);

	void start_accept();

private:
	boost::asio::io_context& m_ctx;
	boost::asio::ip::tcp::acceptor m_acceptor;
};
} // namespace beast_websocket