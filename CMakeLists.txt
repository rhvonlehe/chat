cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(chat CXX)

find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)


add_subdirectory(client)
add_subdirectory(server)
