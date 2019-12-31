#include <asio_generic_server.hpp>
#include <chat_handler.hpp>



int main()
{

    asio_generic_server<chat_handler> server;
    server.start_server(8888);
}
