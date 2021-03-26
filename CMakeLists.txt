cmake_minimum_required(VERSION 3.16.3)

project(PALS CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(ROOT 6.22 CONFIG REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
# add_subdirectory(demo)
