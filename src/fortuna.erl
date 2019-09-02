% Licensed under the Apache License, Version 2.0 (the "License"); you may not
% use this file except in compliance with the License. You may obtain a copy of
% the License at
%
%   http://www.apache.org/licenses/LICENSE-2.0
%
% Unless required by applicable law or agreed to in writing, software
% distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
% WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
% License for the specific language governing permissions and limitations under
% the License.

-module(fortuna).

% -compile(no_native).
-on_load(init/0).

-define(LIBNAME, fortuna).

-export([
    create_context/1,
    acquire_map_context/1
]).

create_context(Id) ->
    Out = nif_create_context(Id),
    io:format("FROM NIF ~p ~n", [Out]).

% fortuna:acquire_map_context(#{db_name => <<"db">>, sig => <<"123">>, map_funs => [<<"hello1">>, <<"hello2">>]}).

acquire_map_context(CtxOpts) -> 
    #{
        db_name := DbName,
        % ddoc_id := DDocId,
        % language := Language,
        sig := Sig,
        % lib := Lib,
        map_funs := MapFuns
    } = CtxOpts,

    Id = <<DbName/binary, Sig/binary>>,

    Ctx = nif_acquire_map_context(Id, MapFuns),
    {ok, Ctx}.
% -callback release_map_context(context()) -> ok | {error, any()}.
% k map_docs(context(), [doc()]) -> {ok, [result()]} | {error, any()}.



init() ->
    io:format("LOADING ~n"),
    PrivDir = case code:priv_dir(?MODULE) of
        {error, _} ->
            EbinDir = filename:dirname(code:which(?MODULE)),
            AppPath = filename:dirname(EbinDir),
            filename:join(AppPath, "priv");
        Path ->
            Path
    end,
    erlang:load_nif(filename:join(PrivDir, ?LIBNAME), 0).


-define(NOT_LOADED, erlang:nif_error({fortuna_nif_not_loaded, ?FILE, ?LINE})).

nif_create_context(_id) -> ?NOT_LOADED.
nif_acquire_map_context(_id, MapFuns) -> ?NOT_LOADED.

