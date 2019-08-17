set(depot_path ${CMAKE_SOURCE_DIR}/deps/depot_tools)

include(ExternalProject)

if (NOT EXISTS ${depot_path}/gclient)
    ExternalProject_add(depot_tools
        GIT_REPOSITORY https://chromium.googlesource.com/chromium/tools/depot_tools.git
        SOURCE_DIR ${depot_path}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
    )
else()
    message("depot_tools is ready")
endif()

set(ENV{PATH} "${depot_path}:$ENV{PATH}")



