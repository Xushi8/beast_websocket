#include <beast_websocket/connection.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <beast_websocket/log.hpp>
#include <beast_websocket/websocket_server.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace beast_websocket
{
connection::connection(boost::asio::io_context& ctx, websocket_server& server) :
	m_ws(ctx), m_uuid(m_gen()), m_websocket_server(server)
{
	m_ws.text(true);
	// m_ws.text(false);
}

boost::uuids::uuid connection::get_uuid() noexcept
{
	return m_uuid;
}

boost::asio::ip::tcp::socket& connection::get_socket() noexcept
{
	return boost::beast::get_lowest_layer(m_ws).socket();
}

void connection::async_accept() noexcept
{
	m_ws.async_accept([self = shared_from_this()](boost::system::error_code ec)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Session {} websocket accept failed, code: {}, message: {}", boost::uuids::to_string(self->get_uuid()), ec.value(), ec.message());
					return;
				}

				spdlog::info("Session {} websocket accept successfully", boost::uuids::to_string(self->get_uuid()));

				self->m_websocket_server.add_conncetion(self);
				self->async_read();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Session {} websocket accept failed, exception: {}", boost::uuids::to_string(self->get_uuid()), e.what());
			}
		});
}

void connection::async_read() noexcept
{
	m_ws.async_read(m_receive_buffer, [self = shared_from_this()](boost::system::error_code ec, [[maybe_unused]] size_t buffer_len)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Session {} receive failed, code: {}, message: {}", boost::uuids::to_string(self->get_uuid()), ec.value(), ec.message());
					self->m_websocket_server.remove_conncetion(self->get_uuid());
					return;
				}

				spdlog::debug("Session {} receive successfully", boost::uuids::to_string(self->get_uuid()));

				// self->m_ws.text(self->m_ws.got_text());
				std::string data = boost::beast::buffers_to_string(self->m_receive_buffer.data());
				self->m_receive_buffer.consume(self->m_receive_buffer.size());
				spdlog::trace("Session {} receive data is: {}", boost::uuids::to_string(self->get_uuid()), data);

				self->async_write(std::move(data));
				self->async_read();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Session {} read failed, exception: {}", boost::uuids::to_string(self->get_uuid()), e.what());
				self->m_websocket_server.remove_conncetion(self->get_uuid());
			}
		});
}

void connection::async_write(std::string msg)
{
	bool is_que_empty;
	{
		std::scoped_lock lock(m_send_mtx);
		is_que_empty = m_send_que.empty();
		m_send_que.emplace(std::move(msg));
	}
	if (is_que_empty)
		async_write();
}

// this function can only be called by async_write(msg) when send_que is not empty
void connection::async_write()
{
	std::string send_data;
	{
		std::scoped_lock lock(m_send_mtx);
		send_data = std::move(m_send_que.front());
		m_send_que.pop();
	}

	m_ws.async_write(boost::asio::const_buffer(send_data.c_str(), send_data.size()),
		[self = shared_from_this()](boost::system::error_code ec, [[maybe_unused]] size_t nsize)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Session {} send failed, code: {}, message: {}", boost::uuids::to_string(self->get_uuid()), ec.value(), ec.message());
					self->m_websocket_server.remove_conncetion(self->get_uuid());
					return;
				}

				spdlog::debug("Session {} send successfully", boost::uuids::to_string(self->get_uuid()));

				bool is_que_empty;
				{
					std::scoped_lock lock(self->m_send_mtx);
					is_que_empty = self->m_send_que.empty();
				}
				if (!is_que_empty)
					self->async_write();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Session {} send failed, code: {}, message: {}", boost::uuids::to_string(self->get_uuid()), ec.value(), ec.message());
				self->m_websocket_server.remove_conncetion(self->get_uuid());
			}
		});
}

} // namespace beast_websocket