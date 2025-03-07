#pragma once

#include <boost/asio/io_context.hpp>
#include <vector>
#include <thread>
#include <cstddef>

namespace beast_websocket
{
struct io_context_pool
{
	io_context_pool(io_context_pool&& that) = delete;

	io_context_pool(size_t thread_num = std::thread::hardware_concurrency());
	
	~io_context_pool() noexcept;

	boost::asio::io_context& get_context();

	void stop();

private:
	
	std::vector<boost::asio::io_context> m_contexts;
	std::vector<boost::asio::io_context::work> m_works;
	std::vector<std::jthread> m_threads;
};
} // namespace beast_websocket