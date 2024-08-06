#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <beast_websocket/message_generated.h>

namespace beast = boost::beast;			// from <boost/beast.hpp>
namespace http = beast::http;			// from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;			// from <boost/asio.hpp>
using tcp = net::ip::tcp;				// from <boost/asio/ip/tcp.hpp>

// Echoes back all received WebSocket messages
void do_session(tcp::socket socket)
{
	try
	{
		websocket::stream<tcp::socket> ws{std::move(socket)};

		ws.accept();

		for (;;)
		{
			beast::flat_buffer buffer;
			ws.read(buffer);

			// Deserialize the message
			auto message = MyGame::Example::GetMessage(buffer.data().data());
			std::cout << "Received message with id: " << message->id() << " and text: " << message->text()->c_str() << std::endl;

			// Echo the message back
			ws.text(ws.got_text());
			ws.write(buffer.data());
		}
	}
	catch (beast::system_error const& se)
	{
		if (se.code() != websocket::error::closed)
			std::cerr << "Error: " << se.code().message() << std::endl;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

int main(int argc, char* argv[])
{
	try
	{

		net::io_context ioc{1};

		tcp::acceptor acceptor{ioc, {tcp::v6(), 8888}};
		for (;;)
		{
			tcp::socket socket{ioc};
			acceptor.accept(socket);

			do_session(std::move(socket));
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
