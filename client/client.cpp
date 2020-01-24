#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace boost;

struct Session
{
    std::shared_ptr<asio::ip::tcp::socket> sock;
    std::string buf;
    std::size_t total_bytes_written;
};

void callback(const boost::system::error_code& ec,
              std::size_t bytes_transferred,
              std::shared_ptr<Session> s)
{
    if (ec.value() != 0)
    {
        std::cout << "Error occured! Error code = "
                  << ec.value()
                  << ". Message: " << ec.message();
        return;
    }
    s->total_bytes_written += bytes_transferred;
    if (s->total_bytes_written == s->buf.length()) {
        return;
    }
    s->sock->async_write_some(
                asio::buffer(
                    s->buf.c_str() +
                    s->total_bytes_written,
                    s->buf.length() -
                    s->total_bytes_written),
                std::bind(callback, std::placeholders::_1,
                          std::placeholders::_2, s));
}

void writeToSocket(std::shared_ptr<asio::ip::tcp::socket> sock)
{
    std::shared_ptr<Session> s(new Session);

    s->buf = std::string("This is the text");
    s->total_bytes_written = 0;
    s->sock = sock;

    sock->async_write_some(asio::buffer(s->buf),
                           std::bind(callback,
                                     std::placeholders::_1,
                                     std::placeholders::_2,
                                     s));


}

int main()
{
#if 0
    asio::io_context ioc;

    asio::ip::tcp protocol = asio::ip::tcp::v4();

    asio::ip::tcp::socket sock(ioc);

    boost::system::error_code ec;

    sock.open(protocol, ec);

    if (ec.value() != 0)
    {
        std::cout << "Failed to open the socket.  Error code = "
                  << ec.value() << ". Message: " << ec.message();
        return ec.value();
    }
#endif

    std::string raw_ip_address = "127.0.0.1";
    unsigned short port_num = 8888;

    asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),
                               port_num);

    asio::io_context ioc;

    auto sock = std::make_shared<asio::ip::tcp::socket>(ioc, ep.protocol());

    try
    {
        sock->connect(ep);
    }
    catch (system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        return e.code().value();
    }


    writeToSocket(sock);
    return 0;
}
