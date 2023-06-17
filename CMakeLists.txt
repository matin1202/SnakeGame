cmake_minimum_required(VERSION 3.12)

project(SnakeGame)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(SOURCES
    ./main.cpp
)

add_executable(SnakeGame ${SOURCES})

target_link_libraries(SnakeGame ${CURSES_LIBRARIES})