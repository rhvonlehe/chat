#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <memory>
#include <vector>
#include <thread>


template<typename ConnectionHandler>
class asio_generic_server
{
    using shared_handler_t = std::shared_ptr<ConnectionHandler>;
public:

    asio_generic_server(int thread_count=1)
        : thread_count_(thread_count),
          acceptor_(io_context_)
    {
    }

    void start_server(uint16_t port)
    {
        auto handler =
                std::make_shared<ConnectionHandler>(io_context_);

        // set up the acceptor to listen on the tcp port
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();

        acceptor_.async_accept(handler->socket(),
                               [=] (auto ec)
        {
            handle_new_connection(handler, ec);
        } );

        // start pool of threads to process the asio events
        for(int i=0; i < thread_count_; ++i)
        {
            thread_pool_.emplace_back( [=]{ io_context_.run(); } );
        }

    }

private:
    void handle_new_connection(shared_handler_t handler,
                               const boost::system::error_code& error)
    {
        if (error) { return; }

        handler->start();

        auto new_handler =
                std::make_shared<ConnectionHandler>(io_context_);

        acceptor_.async_accept( new_handler->socket(),
                                [=](auto ec)
                                {
                                    handle_new_connection(new_handler, ec);
                                }
            );
    }

    int thread_count_;
    std::vector<std::thread> thread_pool_;
    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};
