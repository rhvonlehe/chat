add_executable(chat-server
    main.cpp
    asio_generic_server.cpp
    chat_handler.cpp
    )

target_include_directories(chat-server PRIVATE .)

target_link_libraries(chat-server
    PRIVATE
    Boost::system
    Threads::Threads
    )
