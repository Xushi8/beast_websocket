#include <beast_websocket/log.hpp>
#include <beast_websocket/websocket_server.hpp>
#include <boost/asio/signal_set.hpp>
#include <fmt/format.h>
using fmt::print;

int main()
{
	beast_websocket::set_default_log("beast_websocket", "logs/beast_websocket.txt");

	boost::asio::io_context ctx;

	boost::asio::signal_set signals(ctx, SIGINT, SIGTERM);
	signals.async_wait([&](auto, auto)
		{
			ctx.stop();
		});
	
	beast_websocket::websocket_server server(ctx, 8888);
	server.start_accept();
	
	ctx.run();
	return 0;
}