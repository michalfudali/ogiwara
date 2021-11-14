#include <iostream>
#include <array>
#include <functional>
#include <boost/asio.hpp>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

constexpr uint16_t kServerPort = 68;
constexpr uint16_t kClientPort = 67;

using namespace std::placeholders;

class UdpServer
{
public:
	UdpServer(boost::asio::io_context& io_context)
		: socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), kServerPort))
	{
		StartReceiving();
	}

private:

	void StartReceiving()
	{
		socket_.async_receive_from(
			boost::asio::buffer(receiving_buffer), sender_endpoint_,
			std::bind(&UdpServer::HandleReceive, this, _1, _2));
	}

	void HandleReceive(const std::error_code& error,
					   std::size_t bytes_transferred) 
	{
          BOOST_LOG_TRIVIAL(trace) << "Client has connected.";

		  StartReceiving();
	}

	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	std::array<uint8_t, 512> receiving_buffer;
};

void InitLogging() { 
	 
	boost::log::add_common_attributes();
	boost::log::add_file_log(
      boost::log::keywords::file_name = "/var/log/ogiwara/access.log",
      boost::log::keywords::format = "[%TimeStamp%]: %Message%",
      boost::log::keywords::auto_flush = true
	);
}

int main()
{
	InitLogging();
	BOOST_LOG_TRIVIAL(trace) << "Server has started to work.";
	boost::asio::io_context io_context;
	UdpServer server(io_context);
	io_context.run();
}