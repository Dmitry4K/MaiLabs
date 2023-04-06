cmake_minimum_required(VERSION 3.10)
project(oop_exercise_08)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
add_executable(main main.cpp pentagon.cpp rhombus.cpp trapeze.cpp)
target_link_libraries(main Threads::Threads)