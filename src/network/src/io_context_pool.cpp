#include <beast_websocket/io_context_pool.hpp>
#include <beast_websocket/log.hpp>
#include <random>

namespace beast_websocket
{
io_context_pool::io_context_pool(size_t thread_num) :
	m_contexts(thread_num)
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

io_context_pool::~io_context_pool() noexcept
{
	stop();
	spdlog::info("io_context_pool destroyed");
}

boost::asio::io_context& io_context_pool::get_context()
{
	thread_local std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<size_t> uni(0, m_contexts.size() - 1);
	return m_contexts[uni(rng)];
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