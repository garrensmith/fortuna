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

#include <assert.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "erl_nif.h"


#define ATOM_MAP(NAME) ERL_NIF_TERM ATOM_##NAME
#include "atoms.h"
#undef ATOM_MAP

static ERL_NIF_TERM
nif_create_context(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
  // cool stuff here
//   return enif_make_int(env, 1);
  const char* atom = "hello";
  return enif_make_atom(env, atom);
}

static int
load(ErlNifEnv* env, void** priv, ERL_NIF_TERM info)
{
    return 0;
}

static void
unload(ErlNifEnv* env, void* priv)
{
    return;
}

static ErlNifFunc funcs[] =
{
    // {erl_function_name, erl_function_arity, c_function}
    {"nif_create_context", 1, nif_create_context, ERL_NIF_DIRTY_JOB_CPU_BOUND}
};

ERL_NIF_INIT(fortuna, funcs, &load, NULL, NULL, &unload);