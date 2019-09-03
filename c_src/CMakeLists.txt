
#  taken from https://github.com/couchbase/tlm/blob/master/cmake/Modules/FindCouchbaseErlang.cmake
FIND_PROGRAM(ERLC_EXECUTABLE erlc)
FIND_PROGRAM(ERL_EXECUTABLE erl)
GET_FILENAME_COMPONENT(ERL_REAL_EXE ${ERL_EXECUTABLE} REALPATH)
GET_FILENAME_COMPONENT(ERL_LOCATION ${ERL_REAL_EXE} PATH)

FIND_PATH(ERL_NIF_INCLUDE_PATH erl_nif.h
                HINTS
                ${ERL_LOCATION}/../usr/include
                PATHS
                /usr/lib/erlang/usr/include
                /usr/local/lib/erlang/usr/include
                /opt/local/lib/erlang/usr/include
                /usr/lib64/erlang/usr/include)
SET(ERLANG_INCLUDE_PATH "${ERL_NIF_INCLUDE_PATH}"
    CACHE STRING "Path to Erlang include files")

message("erl_nif.h found at ${ERL_NIF_INCLUDE_PATH}")


add_library(erl_nif MODULE main.cpp atoms.h)
target_link_libraries(erl_nif PRIVATE js_env)
target_link_libraries(erl_nif PRIVATE V8_TARGET)
target_include_directories(erl_nif PUBLIC ${ERLANG_INCLUDE_PATH})

target_link_options(erl_nif BEFORE PRIVATE -undefined dynamic_lookup)

SET(priv_dir ${CMAKE_SOURCE_DIR}/priv/)

# add_custom_command(TARGET erl_nif POST_BUILD 
#   COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
#      "$<TARGET_FILE:erl_nif>"
#      "${priv_dir}/$<TARGET_FILE_NAME:erl_nif>" 
#   COMMENT "Copying to nif to ${priv_dir} directory")

add_custom_command(TARGET erl_nif POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:erl_nif>"
     "${priv_dir}/fortuna$<TARGET_FILE_SUFFIX:erl_nif>" 
  COMMENT "Copying nif to ${priv_dir} directory")
