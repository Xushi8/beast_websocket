#include <beast_websocket/io_context_pool.hpp>
#include <beast_websocket/log.hpp>

namespace beast_websocket
{
io_context_pool::io_context_pool(size_t thread_num) :
	m_contexts(thread_num), next_context(0)
{
	m_works.reserve(thread_num);
	m_threads.reserve(thread_num);
	for (size_t i = 0; i < thread_num; i++)
	{
		m_works.emplace_back(m_contexts[i]);
		m_threads.emplace_back([this, i]
			{
				m_contexts[i].run();
			});
	}
}

io_context_pool::~io_context_pool()
{
	stop();
	spdlog::info("io_context_pool destroyed");
}

boost::asio::io_context& io_context_pool::get_context()
{
	auto& res = m_contexts[next_context];
	if (++next_context == m_contexts.size()) [[unlikely]]
	{
		next_context = 0;
	}
	return res;
}

io_context_pool& io_context_pool::instance()
{
	static io_context_pool singleton;
	return singleton;
}

void io_context_pool::stop()
{
	for (auto& ctx : m_contexts)
	{
		ctx.stop();
	}
	m_works.clear();
	m_threads.clear();
}

} // namespace asio_server