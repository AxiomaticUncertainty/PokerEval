cmake_minimum_required(VERSION 3.22)
project(eval)
add_library(eval hand.cpp monte.cpp)
target_link_libraries(eval game)