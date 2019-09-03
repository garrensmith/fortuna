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

add_executable(tests runner.cpp jsenv.test.cpp jsmanager.test.cpp lru.test.cpp)

target_compile_features(tests PRIVATE cxx_std_14)

# target_link_directories(tests PRIVATE include/catch.hpp)
target_include_directories(tests PUBLIC ./include)
target_include_directories(tests PUBLIC ../jsenv)
target_link_libraries(tests PUBLIC V8_TARGET js_env)
