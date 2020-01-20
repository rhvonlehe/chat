#include <boost/asio.hpp>
#include <iostream>
#include <string>

using namespace boost;

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

    try
    {
        asio::ip::tcp::endpoint ep(asio::ip::address::from_string(raw_ip_address),
                                   port_num);

        asio::io_context ioc;

        asio::ip::tcp::socket sock(ioc, ep.protocol());

        sock.connect(ep);
    }
    catch (system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code()
                  << ". Message: " << e.what();
        return e.code().value();
    }

    return 0;
}
