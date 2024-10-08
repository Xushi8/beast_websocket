#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <boost/unordered/unordered_flat_map.hpp>
#include <boost/uuid/uuid_hash.hpp>
#include <boost/uuid/uuid.hpp>
#include <beast_websocket/io_context_pool.hpp>

namespace beast_websocket
{

struct connection;

struct websocket_server
{
	websocket_server(websocket_server&&) = delete;

	websocket_server(boost::asio::io_context& ctx, uint16_t port);

	void start_accept();

	void add_conncetion(std::shared_ptr<connection> connection_ptr);
	
	void remove_conncetion(boost::uuids::uuid uuid);

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::unordered_flat_map<boost::uuids::uuid, std::shared_ptr<connection>> m_conncetions;
    inline static io_context_pool m_context_pool;
};
} // namespace beast_websocket