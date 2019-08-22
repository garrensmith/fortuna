cmake_minimum_required(VERSION 3.15)

add_executable(tests runner.cpp jsenv.test.cpp jsmanager.test.cpp)

target_compile_features(tests PRIVATE cxx_std_14)

# target_link_directories(tests PRIVATE include/catch.hpp)
target_include_directories(tests PUBLIC ./include)
target_include_directories(tests PUBLIC ../src)
target_link_libraries(tests PUBLIC V8_TARGET js_env)
