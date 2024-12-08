# C++ 23 release mode with debug info
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O2")

project(poker-eval)

add_executable(poker-eval.t.tsk main.t.cpp)
add_executable(poker-eval.tsk main.cpp)

# add subdirs
add_subdirectory(game)
add_subdirectory(eval)

# add linked libs
target_link_libraries(poker-eval.t.tsk game eval)
target_link_libraries(poker-eval.tsk game eval)