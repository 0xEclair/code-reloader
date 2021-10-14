cmake_minimum_required(VERSION 3.15)
project(code_reloader)

set(CMAKE_CXX_STANDARD 20)

add_library(lib SHARED lib/lib.hpp)
set_target_properties(lib PROPERTIES LINKER_LANGUAGE CXX)

add_library(newlib SHARED lib/newlib.cc)
set_target_properties(newlib PROPERTIES LINKER_LANGUAGE CXX)

add_executable(code_reloader main.cpp)
target_link_libraries(code_reloader dl)