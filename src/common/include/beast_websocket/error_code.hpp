#pragma once

#include <system_error>

namespace beast_websocket
{
enum class error_code
{
	success = 0,
	network_error,
	file_error,
};

struct error_category : public std::error_category
{
	const char* name() const noexcept override
	{
		return "beast_websocket::error_category";
	}

	std::string message(int ev) const noexcept override
	{
		switch (static_cast<error_code>(ev))
		{
		case error_code::success:
			return "Success";
			break;
		case error_code::network_error:
			return "Newwork error";
			break;
		case error_code::file_error:
			return "File error";
			break;
		default:
			return "Unkown error";
			break;
		}
	}
};

[[nodiscard]] error_category const& get_error_category() noexcept;

inline std::error_code make_error_code(error_code ec) noexcept
{
	return {static_cast<int>(ec), get_error_category()};
}

void test_error_code();

} // namespace beast_websocket

template <>
struct std::is_error_code_enum<beast_websocket::error_code> : public std::true_type
{
};
