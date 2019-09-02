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

cmake_minimum_required(VERSION 3.14)

# This isn't quite right but can't find a better way for this to work
include_directories(${CMAKE_BINARY_DIR}/js)

add_library(js_env STATIC
        jsenv.h
        jsenv.cpp
        jsenv_manager.h
        jsenv_manager.cpp
        lru.h)

target_link_libraries(js_env PRIVATE V8_TARGET)

add_dependencies(js_env JS_HEADERS)

add_executable(main main.cpp V8init.h)

target_link_libraries(main PRIVATE V8_TARGET js_env)

target_compile_features(main PRIVATE cxx_std_14)
