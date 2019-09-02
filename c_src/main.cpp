// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

// #include <assert.h>
// #include <math.h>
// #include <string.h>
// #include <stdio.h>
#include <cstdlib>
#include <iostream>

#include "erl_nif.h"

#include "V8init.h"
#include "jsenv_manager.h"

#define ATOM_MAP(NAME) ERL_NIF_TERM ATOM_##NAME
#include "atoms.h"
#undef ATOM_MAP

V8Init* v8Init;
JSEnvManager* jsenvs;

ERL_NIF_TERM make_atom(ErlNifEnv* env, const char* atom) {
    ERL_NIF_TERM ret;

    if(!enif_make_existing_atom(env, atom, &ret, ERL_NIF_LATIN1))
    {
        return enif_make_atom(env, atom);
    }

    return ret;
}

bool make_string(ErlNifEnv* env, ERL_NIF_TERM bin, std::string& val) {
    ErlNifBinary raw;
    if(!enif_inspect_binary(env, bin, &raw)) {
        return false;
    }
	val = std::string(reinterpret_cast<char*>(raw.data), raw.size);
    return true;
}

static ERL_NIF_TERM nif_create_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 1) {
		return enif_make_badarg(env);
	}

    std::string id;
    if (!make_string(env, argv[0], id)) {
        return enif_make_badarg(env);
    }

    jsenvs->createEnv(id);

    auto contextMap = enif_make_new_map(env);
    auto key = make_atom(env, "context_id");
    auto val = enif_make_string(env, id.c_str(), ERL_NIF_LATIN1);
    enif_make_map_put(env, contextMap, key, val, &contextMap);

    return contextMap;
}

static ERL_NIF_TERM nif_acquire_map_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 2) {
		return enif_make_badarg(env);
	}

    std::string id;
    if (!make_string(env, argv[0], id)) {
        return enif_make_badarg(env);
    }

    if(!enif_is_list(env, argv[1])) {
        return enif_make_badarg(env);
    }

    auto list = argv[1];
    ERL_NIF_TERM erlMapFun;
     std::vector<std::string> mapFuns {};

    while(enif_get_list_cell(env, list, &erlMapFun, &list)) {
        std::string fun;
        make_string(env, erlMapFun, fun);
        std::cout << "fun " << fun << std::endl;
        mapFuns.push_back(fun);
    }

    auto jsenv = jsenvs->getOrCreateEnv(id);

    ERL_NIF_TERM id_term;

    // convert the C char array string into a elixir string (erlang binary)
    unsigned char *id_term_bin = enif_make_new_binary(env, id.length(), &id_term);
    strncpy((char*)id_term_bin, id.c_str(), id.length());

    auto contextMap = enif_make_new_map(env);
    auto key = make_atom(env, "context_id");
    enif_make_map_put(env, contextMap, key, id_term, &contextMap);
    return contextMap;
}

static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
    std::cout << "LOADING PLATFORM" << std::endl;
	v8Init = new V8Init();
    v8Init->initialisePlatform();
	jsenvs = new JSEnvManager();
	return 0;
}

static void unload(ErlNifEnv* env, void* priv) {
	if (v8Init != nullptr) {
		v8Init->shutdownPlatform();
	}
	return;
}

static ErlNifFunc funcs[] = {
	// {erl_function_name, erl_function_arity, c_function}
	{ "nif_create_context", 1, nif_create_context, ERL_NIF_DIRTY_JOB_CPU_BOUND },
	{ "nif_acquire_map_context", 2, nif_acquire_map_context, ERL_NIF_DIRTY_JOB_CPU_BOUND }
};

ERL_NIF_INIT(fortuna, funcs, &load, NULL, NULL, &unload);