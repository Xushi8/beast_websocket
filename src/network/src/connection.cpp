#include <beast_websocket/connection.hpp>
#include <beast_websocket/connection_manager.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <beast_websocket/log.hpp>
#include <system_error>

namespace beast_websocket
{
connection::connection(boost::asio::io_context& ctx) :
	m_ctx(ctx), m_ws(ctx), m_uuid(m_gen())
{
}

boost::uuids::uuid connection::get_uuid() noexcept
{
	return m_uuid;
}

boost::asio::ip::tcp::socket& connection::get_socket() noexcept
{
	return boost::beast::get_lowest_layer(m_ws).socket();
}

void connection::async_accept()
{
	m_ws.async_accept([self = shared_from_this()](std::error_code ec)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Websocket accept failed, code: {}, message: {}", ec.value(), ec.message());
					return;
				}

				connection_manager::instance().add_conncetion(self);
				self->start();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Websocket accept failed, exception: {}", e.what());
			}
		});
}

void connection::start()
{
	m_ws.async_read(m_receive_buffer, [self = shared_from_this()](std::error_code ec, [[maybe_unused]] size_t buffer_len)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Websocket receive failed, code: {}, message: {}", ec.value(), ec.message());
					connection_manager::instance().remove_conncetion(self->get_uuid());
					return;
				}

				self->m_ws.text(self->m_ws.got_text());
				std::string data = boost::beast::buffers_to_string(self->m_receive_buffer.data());
				self->m_receive_buffer.consume(self->m_receive_buffer.size());
				spdlog::info("Websocket receive data is: {}", data);

				self->async_send(std::move(data));
				self->start();
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Websocket read failed, exception: {}", e.what());
				connection_manager::instance().remove_conncetion(self->get_uuid());
			}
		});
}

// 有点问题, 回头用条件变量重写一下
void connection::async_send(std::string msg)
{
	{
		std::scoped_lock lock(m_send_mtx);
		size_t que_len = m_send_que.size();
		m_send_que.emplace(msg);
		if (que_len > 0)
		{
			return;
		}
	}

	m_ws.async_write(boost::asio::const_buffer(msg.c_str(), msg.size()), [self = shared_from_this()](std::error_code ec, [[maybe_unused]] size_t nsize)
		{
			try
			{
				if (ec)
				{
					spdlog::info("Websocket send failed, code: {}, message: {}", ec.value(), ec.message());
					connection_manager::instance().remove_conncetion(self->get_uuid());
					return;
				}

				std::string send_msg;
				{
					std::scoped_lock lock(self->m_send_mtx);
					self->m_send_que.pop();
					if (self->m_send_que.empty())
					{
						return;
					}
					send_msg = self->m_send_que.front();
				}

				self->async_send(std::move(send_msg));
			}
			catch (std::exception const& e)
			{
				spdlog::warn("Websocket send failed, code: {}, message: {}", ec.value(), ec.message());
				connection_manager::instance().remove_conncetion(self->get_uuid());
			}
		});
}

} // namespace beast_websocket