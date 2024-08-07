#pragma once

#include <memory>
#include <boost/beast/websocket/stream.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <queue>
#include <mutex>

namespace beast_websocket
{

struct websocket_server;

struct connection : public std::enable_shared_from_this<connection>
{
	connection(boost::asio::io_context& ctx, websocket_server& server);

	boost::uuids::uuid get_uuid() noexcept;

	boost::asio::ip::tcp::socket& get_socket() noexcept;

	void async_accept() noexcept;

private:
	void async_read() noexcept;

	void async_write(std::string msg);

	void async_write();

	boost::beast::websocket::stream<boost::beast::tcp_stream> m_ws;
	boost::uuids::uuid m_uuid;

	boost::beast::flat_buffer m_receive_buffer;
	std::queue<std::string> m_send_que;
	std::mutex m_send_mtx;

	websocket_server& m_websocket_server;

	static inline boost::uuids::random_generator m_gen;
};
} // namespace beast_websocket