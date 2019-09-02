
#  taken from https://github.com/couchbase/tlm/blob/master/cmake/Modules/FindCouchbaseErlang.cmake
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


add_library(erl_nif SHARED main.c atoms.h)
target_include_directories(erl_nif PUBLIC ${ERLANG_INCLUDE_PATH})

target_link_options(erl_nif BEFORE PRIVATE -undefined dynamic_lookup)

SET(priv_dir ${CMAKE_SOURCE_DIR}/priv/)

add_custom_command(TARGET erl_nif POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy_if_different 
     "$<TARGET_FILE:erl_nif>"
     "${priv_dir}/$<TARGET_FILE_NAME:erl_nif>" 
  COMMENT "Copying to nif to ${priv_dir} directory")
