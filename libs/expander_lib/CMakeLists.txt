cmake_minimum_required(VERSION 3.13)

project(expander_lib)

add_library(expander_lib expander_lib.c)

target_include_directories(expander_lib PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/inc
)

target_link_libraries(expander_lib
    pico_stdlib
    hardware_i2c
)