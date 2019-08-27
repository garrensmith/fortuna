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

# file(COPY . 
#     DESTINATION ${PROJECT_BINARY_DIR}/js
#     FILES_MATCHING PATTERN "*.js"
#     )

find_program(NODE node)

set(HEADIFY ${CMAKE_CURRENT_SOURCE_DIR}/headify.js)
set(ESCODEGEN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/escodegen.js)
set(ESCODEGEN_H ${CMAKE_CURRENT_BINARY_DIR}/escodegen.h)
set(REWRITE_ANON_FUN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/rewrite_anon_fun.js)
set(REWRITE_ANON_FUN_H ${CMAKE_CURRENT_BINARY_DIR}/rewrite_anon_fun.h)
set(ATELES_MAP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/ateles_map.js)
set(ATELES_MAP_H ${CMAKE_CURRENT_BINARY_DIR}/ateles_map.h)
set(ESPRIMA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/esprima.js)
set(ESPRIMA_H ${CMAKE_CURRENT_BINARY_DIR}/esprima.h)

message("WRIT ${ESCODEGEN_H} ${CMAKE_CURRENT_BINARY_DIR}")
# set(ESPRIMA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/js/esprima.js)
# set(ESPRIMA_HDR ${CMAKE_CURRENT_BINARY_DIR}/esprima.h)
# set(REWRITE_ANON_FUN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/js/rewrite_anon_fun.js)
# set(REWRITE_ANON_FUN_HDR ${CMAKE_CURRENT_BINARY_DIR}/rewrite_anon_fun.h)
# set(ATELES_MAP_SRC ${CMAKE_CURRENT_SOURCE_DIR}/js/ateles_map.js)
# set(ATELES_MAP_HDR ${CMAKE_CURRENT_BINARY_DIR}/ateles_map.h)

add_custom_command(
    OUTPUT ${ESCODEGEN_H}
    COMMAND ${NODE} ${HEADIFY} ${ESCODEGEN_SRC} ${ESCODEGEN_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "RUNNING ${ESCODEGEN_SRC}"
    DEPENDS ${ESCODEGEN_SRC}
)

add_custom_command(
    OUTPUT ${REWRITE_ANON_FUN_H}
    COMMAND ${NODE} ${HEADIFY} ${REWRITE_ANON_FUN_SRC} ${REWRITE_ANON_FUN_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "RUNNING ${REWRITE_ANON_FUN_SRC}"
    DEPENDS ${REWRITE_ANON_FUN_SRC}
)

add_custom_command(
    OUTPUT ${ATELES_MAP_H}
    COMMAND ${NODE} ${HEADIFY} ${ATELES_MAP_SRC} ${ATELES_MAP_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "RUNNING ${ATELES_MAP_SRC}"
    DEPENDS ${ATELES_MAP_SRC}
)

add_custom_command(
    OUTPUT ${ESPRIMA_H}
    COMMAND ${NODE} ${HEADIFY} ${ESPRIMA_SRC} ${ESPRIMA_H}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "RUNNING ${ESPRIMA_H}"
    DEPENDS ${ESPRIMA_SRC}
)

add_custom_target(
  JS_HEADERS
  DEPENDS ${ESCODEGEN_H} ${REWRITE_ANON_FUN_H} ${ATELES_MAP_H} ${ESPRIMA_H}
)

