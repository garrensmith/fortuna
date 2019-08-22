# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.

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



