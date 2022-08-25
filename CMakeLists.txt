cmake_minimum_required(VERSION 3.20)
project(cropcpp_parent)

set(CMAKE_CXX_STANDARD 14 -std=c++11)

add_executable(parent
        main.cpp)

add_subdirectory(src)
add_subdirectory(test)
