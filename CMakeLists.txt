cmake_minimum_required(VERSION 4.0)
project(blue_prince_code)

set(CMAKE_CXX_STANDARD 26)


add_executable(blue_prince_code src/main.cpp)
find_package(fmt REQUIRED)
target_link_libraries(blue_prince_code fmt::fmt-header-only)