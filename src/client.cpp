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

// Function to create a serialized FlatBuffer message
std::vector<uint8_t> create_flatbuffer_message(int id, const std::string& text)
{
	flatbuffers::FlatBufferBuilder builder;

	auto text_offset = builder.CreateString(text);

	auto message = MyGame::Example::CreateMessage(builder, id, text_offset);

	builder.Finish(message);

	return {builder.GetBufferPointer(), builder.GetBufferPointer() + builder.GetSize()};
}

int main(int argc, char** argv)
{
	try
	{
		// Check command line arguments.
		if (argc != 3)
		{
			std::cerr << "Usage: websocket_client <host> <port>\n";
			return EXIT_FAILURE;
		}
		auto const host = argv[1];
		auto const port = argv[2];

		// The io_context is required for all I/O
		net::io_context ioc;

		// These objects perform our I/O
		tcp::resolver resolver{ioc};
		websocket::stream<tcp::socket> ws{ioc};

		// Look up the domain name
		auto const results = resolver.resolve(host, port);

		// Make the connection on the IP address we get from a lookup
		net::connect(ws.next_layer(), results.begin(), results.end());

		// Perform the WebSocket handshake
		ws.handshake(host, "/");

		// Create a FlatBuffer message
		auto message = create_flatbuffer_message(1, "Hello, WebSocket!");

		// Send the message
		ws.write(net::buffer(message));

		// This buffer will hold the incoming message
		beast::flat_buffer buffer;

		// Read a message into our buffer
		ws.read(buffer);

		// Deserialize the message
		auto received_message = MyGame::Example::GetMessage(buffer.data().data());
		std::cout << "Received message with id: " << received_message->id() << " and text: " << received_message->text()->c_str() << std::endl;

		// Close the WebSocket connection
		ws.close(websocket::close_code::normal);
	}
	catch (beast::system_error const& se)
	{
		std::cerr << "Error: " << se.code().message() << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
