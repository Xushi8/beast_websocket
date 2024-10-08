#include <beast_websocket/connection.hpp>
#include <beast_websocket/websocket_server.hpp>
#include <beast_websocket/log.hpp>
#include <beast_websocket/io_context_pool.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace beast_websocket
{
websocket_server::websocket_server(boost::asio::io_context& ctx, uint16_t port) :
	m_acceptor(ctx, {boost::asio::ip::tcp::v6(), port})
{
	spdlog::info("Server start on port: {}", port);
}

void websocket_server::start_accept()
{
	// auto connection_ptr = std::make_shared<connection>(m_ctx);
	auto connection_ptr = std::make_shared<connection>(m_context_pool.get_context(), *this);
	m_acceptor.async_accept(connection_ptr->get_socket(), [this, connection_ptr](boost::system::error_code ec)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Acceptor async_accept failed, code: {}, message: {}", ec.value(), ec.message());
					start_accept();
					return;
				}

				spdlog::debug("Acceptor async_accept successfully, session: {}", boost::uuids::to_string(connection_ptr->get_uuid()));

				connection_ptr->async_accept();

				start_accept();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Acceptor async_accept exceptino: {}", e.what());
			}
		});
}

void websocket_server::add_conncetion(std::shared_ptr<connection> connection_ptr)
{
	m_conncetions.try_emplace(connection_ptr->get_uuid(), connection_ptr);
}

void websocket_server::remove_conncetion(boost::uuids::uuid uuid)
{
	m_conncetions.erase(uuid);
}

} // namespace beast_websocket