#ifndef CHAT_HANDLER_HPP
#define CHAT_HANDLER_HPP

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <deque>


class chat_handler : public std::enable_shared_from_this<chat_handler>
{
    using bsock = boost::asio::ip::tcp::socket;
    using bcontext = boost::asio::io_context;
    using bstrand = boost::asio::io_context::strand;

public:
    chat_handler(bcontext& context)
        : context_(context),
          socket_(context),
          write_strand_(context)
    {}

    void send(std::string msg);

    bsock& socket()
    {
        return socket_;
    }

    void start()
    {
        read_packet();
    }

private:
    void read_packet(void);
    void read_packet_done(boost::system::error_code const& error,
                          std::size_t bytes_transferred);
    void queue_message(std::string message);
    void start_packet_send(void);
    void packet_send_done(boost::system::error_code const& error);


    bcontext& context_;
    bsock socket_;
    bstrand write_strand_;
    boost::asio::streambuf in_packet_;
    std::deque<std::string> send_packet_queue;


};

#endif // CHAT_HANDLER_HPP
