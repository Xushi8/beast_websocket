#include <beast_websocket/connection.hpp>
#include <beast_websocket/websocket_server.hpp>
#include <beast_websocket/log.hpp>

namespace beast_websocket
{
websocket_server::websocket_server(boost::asio::io_context& ctx, uint16_t port) :
	m_ctx(ctx), m_acceptor(ctx, {boost::asio::ip::tcp::v6(), port})
{
	spdlog::info("Server start on port: {}", port);
}

void websocket_server::start_accept()
{
	auto conptr = std::make_shared<connection>(m_ctx);
	m_acceptor.async_accept(conptr->get_socket(), [this, conptr](std::error_code ec)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Acceptor async_accept failed, code: {}, message: {}", ec.value(), ec.message());
					return;
				}

				conptr->async_accept();
				
				start_accept();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Acceptor async_accept exceptino: {}", e.what());
			}
    });
}
} // namespace beast_websocket