add_executable(chat-client client.cpp)

target_include_directories(chat-client PRIVATE .)

target_link_libraries(chat-client
    PRIVATE
    Boost::system
    Threads::Threads
    )
