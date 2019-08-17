
set(DEPS_PATH ${CMAKE_SOURCE_DIR}/deps)
set(V8_PATH ${DEPS_PATH}/v8)
set(V8_HDRS ${V8_PATH}/include)
set(V8_LIB ${V8_PATH}/out.gn/x64.release/obj)

message("hello v8 ${V8_LIB}")

if (NOT EXISTS ${V8_HDRS})
    execute_process(
        COMMAND fetch v8
        WORKING_DIRECTORY ${DEPS_PATH}
    )

    execute_process(
        COMMAND git checkout refs/tags/7.8.86
        COMMAND gclient sync -D
        WORKING_DIRECTORY ${V8_PATH}
    )
endif()

if (NOT EXISTS ${V8_LIB}/libv8_libbase.a)
    message("HERE")


    execute_process(
        COMMAND ./tools/dev/v8gen.py x64.release -- v8_monolithic=true v8_use_external_startup_data=false use_custom_libcxx=false
        COMMAND ninja -C out.gn/x64.release
        WORKING_DIRECTORY ${V8_PATH}
    )


else()
    message("v8 built")
endif()