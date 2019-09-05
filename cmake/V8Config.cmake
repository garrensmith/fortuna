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

set(DEPS_PATH ${CMAKE_SOURCE_DIR}/deps)
set(V8_PATH ${DEPS_PATH}/v8)
set(V8_HDRS ${V8_PATH}/include)
set(V8_LIB ${V8_PATH}/out.gn/x64.release/obj)

if (NOT EXISTS ${V8_HDRS})
    message("fetching v8")
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
    message("Building v8")


    execute_process(
        COMMAND tools/dev/v8gen.py x64.release -- v8_monolithic=true v8_use_external_startup_data=false use_custom_libcxx=false
        WORKING_DIRECTORY ${V8_PATH}
    )

    execute_process(
        COMMAND ninja -C out.gn/x64.release
        WORKING_DIRECTORY ${V8_PATH}
    )


else()
    message("v8 built")
endif()