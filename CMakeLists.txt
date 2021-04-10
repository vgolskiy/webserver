cmake_minimum_required(VERSION 3.19)
project(webserver)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserver main.cpp convertConfig.cpp server_tools.cpp utils.cpp readConfig.cpp Request.cpp Server.cpp )