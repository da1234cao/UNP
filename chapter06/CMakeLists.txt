
cmake_minimum_required(VERSION 3.11)

SET(CMAKE_BUILD_TYPE "Debug")

project(server)

find_package(
    Boost REQUIRED
    COMPONENTS log)

include_directories(${Boost_INCLUDE_DIRS} ../include)

add_executable(${PROJECT_NAME} select_demo.cpp)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
