#include <array>
#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>

constexpr uint16_t kServerPort = 68;
constexpr uint16_t kClientPort = 67;

namespace asio = boost::asio;

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

void InitLogging() {
  boost::log::add_common_attributes();
  boost::log::add_file_log(
      boost::log::keywords::file_name = "/var/log/ogiwara/access.log",
      boost::log::keywords::format = "[%TimeStamp%]: %Message%",
      boost::log::keywords::auto_flush = true);
}

asio::awaitable<void> HandleClient(
    std::shared_ptr<asio::ip::udp::socket> socket) {
  asio::ip::udp::endpoint remote_endpoint_;
  std::array<uint8_t, 512> buffer;

  std::cout << "Received: "
            << co_await socket->async_receive(asio::buffer(buffer),
                                              asio::use_awaitable);
}

asio::awaitable<void> listener() {
  auto local_executor = co_await asio::this_coro::executor;
  auto socket = std::make_shared<asio::ip::udp::socket>(
      local_executor,
      asio::ip::udp::endpoint(asio::ip::udp::v4(), kServerPort));
  while (true) {
    co_await socket->async_wait(asio::ip::udp::socket::wait_read,
                                asio::use_awaitable);
    co_spawn(local_executor, HandleClient(socket), asio::detached);
  }
}

int main() {
  InitLogging();

  asio::io_context io_context;

  asio::signal_set signals(io_context, SIGINT, SIGTERM);
  signals.async_wait([&](auto, auto) { io_context.stop(); });

  asio::co_spawn(io_context, listener(), asio::detached);

  io_context.run();
}