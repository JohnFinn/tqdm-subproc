cmake_minimum_required(VERSION 3.25)
project(foo)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(main main.cpp)

find_package(fmt)
target_link_libraries(main PRIVATE fmt::fmt)
