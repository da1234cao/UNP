cmake_minimum_required(VERSION 3.11)

SET(CMAKE_BUILD_TYPE "Debug")

project(udp_demo)

add_executable(udp_client udp_client.cpp)

add_executable(udp_server udp_server.cpp)