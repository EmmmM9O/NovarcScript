#include <boost/asio.hpp>

int main() {
  int goal = 150, score = 0, happiness;
  boost::asio::io_service io_service;
  boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(5));
  timer.async_wait([&](const boost::system::error_code &ec) -> void {
    if (happiness <= 50) return;
    if (goal <= score) score++;
  });
  timer.async_wait(
      [&](const boost::system::error_code &ec) -> void { happiness++; });
  io_service.run();
}