#pragma once

#include <boost/asio/io_context.hpp>
#include <vector>
#include <thread>
#include <cstddef>

namespace beast_websocket
{
struct io_context_pool
{
	io_context_pool(io_context_pool&&) = delete;

	~io_context_pool() noexcept;

	boost::asio::io_context& get_context();

	void stop();

	static io_context_pool& instance();

private:
	io_context_pool(size_t thread_num = std::thread::hardware_concurrency());
	
	std::vector<boost::asio::io_context> m_contexts;
	std::vector<boost::asio::io_context::work> m_works;
	std::vector<std::jthread> m_threads;
	size_t m_next_context;
};
} // namespace beast_websocket