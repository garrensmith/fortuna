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
-behavior(couch_eval).

% -compile(no_native).
-on_load(init/0).

-define(LIBNAME, fortuna).

-include_lib("eunit/include/eunit.hrl").
-include_lib("couch_mrview/include/couch_mrview.hrl").
-include_lib("couch/include/couch_db.hrl").

-export([
    acquire_map_context/1,
    release_map_context/1,
    map_docs/2
]).


acquire_map_context(CtxOpts) -> 
    #{
        db_name := DbName,
        sig := Sig,
        map_funs := MapFuns
    } = CtxOpts,

    Id = <<DbName/binary, Sig/binary>>,
    nif_acquire_map_context(Id, MapFuns),
    {ok, #{context_id => Id}}.


release_map_context(Ctx) ->
    #{
        context_id := Id
    } = Ctx,
    nif_release_context(Id).


map_docs(Ctx, Docs) ->
    #{
        context_id := Id
    } = Ctx,
    {ok, lists:map(fun(Doc) ->
        Json = couch_doc:to_json_obj(Doc, []),
        StringJson = jiffy:encode(Json),
        {ok, ResultString} = nif_map_doc(Id, StringJson),
        Tupled = lists:map(fun(ViewResult) ->
            lists:map(fun([K, V]) -> {K, V} end, ViewResult)
        end, jiffy:decode(ResultString)),
        {Doc#doc.id, Tupled}
    end, Docs)}.
    

init() ->
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

nif_acquire_map_context(_id, MapFuns) -> ?NOT_LOADED.
nif_release_context(_id) -> ?NOT_LOADED.
nif_map_doc(_id, _Doc) -> ?NOT_LOADED.

