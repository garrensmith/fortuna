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

#include <cstdlib>
#include <iostream>

#include "erl_nif.h"

#include "V8init.h"
#include "jsenv_manager.h"

// #define ATOM_MAP(NAME) ERL_NIF_TERM ATOM_##NAME
// #include "atoms.h"
// #undef ATOM_MAP

V8Init* v8Init;
JSEnvManager* jsenvs;

ERL_NIF_TERM t2(ErlNifEnv* env, ERL_NIF_TERM a, ERL_NIF_TERM b)
{
    return enif_make_tuple2(env, a, b);
}

ERL_NIF_TERM make_atom(ErlNifEnv* env, const char* atom) {
    ERL_NIF_TERM ret;

    if(!enif_make_existing_atom(env, atom, &ret, ERL_NIF_LATIN1))
    {
        return enif_make_atom(env, atom);
    }

    return ret;
}

bool enif_to_string(ErlNifEnv* env, ERL_NIF_TERM bin, std::string& val) {
    ErlNifBinary raw;
    if(!enif_inspect_binary(env, bin, &raw)) {
        return false;
    }
	val = std::string(reinterpret_cast<char*>(raw.data), raw.size);
    return true;
}

static ERL_NIF_TERM make_string(ErlNifEnv* env, std::string& val) {
    ERL_NIF_TERM term;

    // convert the C char array string into a elixir string (erlang binary)
    unsigned char *term_bin = enif_make_new_binary(env, val.length(), &term);
    strncpy((char*)term_bin, val.c_str(), val.length());

    return term;
}

static ERL_NIF_TERM nif_acquire_map_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 2) {
		return enif_make_badarg(env);
	}

    std::string id;
    if (!enif_to_string(env, argv[0], id)) {
        return enif_make_badarg(env);
    }

    if(!enif_is_list(env, argv[1])) {
        return enif_make_badarg(env);
    }

    auto jsenv = jsenvs->getOrCreateEnv(id);

    int i = 0;
    ERL_NIF_TERM list = argv[1];
    ERL_NIF_TERM cell;

    while(enif_get_list_cell(env, list, &cell, &list)) {
        auto fun = std::string("");
        if (!enif_to_string(env, cell, fun)) {
            return t2(env, make_atom(env, "error"), make_atom(env, "could not add functions"));
        }
        try {
            jsenv->addFun(std::to_string(i), fun);
            i++;
        } catch (std::string err) {
            jsenvs->deleteEnv(id);
            return t2(env, make_atom(env, "error"), make_string(env, err));
        }
    }

    return make_atom(env, "ok");
}

static ERL_NIF_TERM nif_release_context(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 1) {
		return enif_make_badarg(env);
	}

    std::string id;
    if (!enif_to_string(env, argv[0], id)) {
        return enif_make_badarg(env);
    }

    try {
        jsenvs->deleteEnv(id);
        return make_atom(env, "ok");
    } catch (std::string err) {
        return t2(env, make_atom(env, "error"), make_string(env, err));
    }
}

static ERL_NIF_TERM nif_map_doc(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
	if (argc != 2) {
		return enif_make_badarg(env);
	}

    std::string id;
    if (!enif_to_string(env, argv[0], id)) {
        return enif_make_badarg(env);
    }

    if(!enif_is_binary(env, argv[1])) {
        return enif_make_badarg(env);
    }

    auto jsenv = jsenvs->getOrCreateEnv(id);

    std::string doc;
    if (!enif_to_string(env, argv[1], doc)) {
        auto err = std::string("could not read doc");
        return t2(env, make_atom(env, "error"), make_string(env, err));
    }

    try {
        auto results = jsenv->mapDoc(doc);
        return t2(env, make_atom(env, "ok"), make_string(env, results));
    } catch (std::string err) {
        return t2(env, make_atom(env, "error"), make_string(env, err));
    }

}


static int load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info) {
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
	{ "nif_acquire_map_context", 2, nif_acquire_map_context, ERL_NIF_DIRTY_JOB_CPU_BOUND },
    { "nif_release_context", 1, nif_release_context, ERL_NIF_DIRTY_JOB_CPU_BOUND},
	{ "nif_map_doc", 2, nif_map_doc, ERL_NIF_DIRTY_JOB_CPU_BOUND }

};

ERL_NIF_INIT(fortuna, funcs, &load, NULL, NULL, &unload);