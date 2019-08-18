cmake_minimum_required(VERSION 3.15)

add_executable(runner runner.cpp)

target_compile_features(runner PRIVATE cxx_std_14)

target_link_directories(runner PRIVATE include/catch.hpp)
