cmake_minimum_required(VERSION 3.15)
project (fortuna)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

include(GetDepotTools)

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(js)

# set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# include(FetchContent)

# FetchContent_Declare(
#   v8
#   GIT_REPOSITORY https://github.com/v8/v8.git
#   GIT_TAG        7.8.86
# )

# FetchContent_MakeAvailable(v8)

# FetchContent_Declare(
#     depot_tools
#     GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
# )

# FetchContent_MakeAvailable(depot_tools)

# execute_process(
#     COMMAND ./gclient
#     WORKING_DIRECTORY ${depot_tools_SOURCE_DIR}
# )

# set(ENV{PATH} ${depot_tools_SOURCE_DIR})
# set(ENV{PATH} "${depot_tools_SOURCE_DIR}:$ENV{PATH}")


# set(V8_TEST ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8)

# if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8 )
#     file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps)
#     execute_process(
#         COMMAND fetch v8
#         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps
#     )
# endif()

# SETUP
# execute_process(
#     COMMAND git checkout refs/tags/7.8.86
#     COMMAND gclient sync -D
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8
# )

# BUILD
# execute_process(
#     COMMAND ./tools/dev/v8gen.py x64.release -- v8_monolithic=true v8_use_external_startup_data=false use_custom_libcxx=false
#     COMMAND ninja out.gn/x64.release
#     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8
# )

# file(GLOB SOURCES "src/*.cpp")

# # add_executable(fortuna ${SOURCES})
# add_executable(hello src/v8hello.cpp)

# set(V8_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/include)
# set(V8_LIB ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/out.gn/x64.release/obj)
# # set(V8_HDRS ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/include/libplatform)
# set(ICU ${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/out.gn/x64.release/obj/third_party/icu)


# include_directories(${V8_HDRS})
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/v8/include/libplatform)
# target_link_libraries(hello ${V8_LIB})
# target_link_libraries(hello ${V8_LIB}/libv8_libbase.a)
# target_link_libraries(hello ${V8_LIB}/libv8_libplatform.a)
# target_link_libraries(hello ${V8_LIB}/libv8_monolith.a)

# target_link_libraries(hello ICU)

