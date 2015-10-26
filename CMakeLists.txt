cmake_minimum_required(VERSION 3.3)
project(producerconsumer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(SOURCE_FILES buffer.c buffer.h producerconsumer.c)
add_executable(producerconsumer ${SOURCE_FILES})