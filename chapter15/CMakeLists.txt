cmake_minimum_required(VERSION 3.11)

SET(CMAKE_BUILD_TYPE "Debug")

project(unix_demo)

add_executable(unix_client unixCli.cpp)

add_executable(unix_server unixSer.cpp)