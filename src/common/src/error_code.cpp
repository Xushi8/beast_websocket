#include <beast_websocket/error_code.hpp>

#include <fmt/core.h>
using fmt::print;

namespace beast_websocket
{

[[nodiscard]] error_category const& get_error_category() noexcept
{
	static error_category error_category_instance;
	return error_category_instance;
}

void test_error_code()
{
	std::error_code ec = beast_websocket::error_code::file_error;
	print("value: {}, message: {}.\n", ec.value(), ec.message());
}
} // namespace beast_websocket