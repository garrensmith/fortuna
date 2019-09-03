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

-module(context_tests).

-include_lib("eunit/include/eunit.hrl").
-include_lib("couch/include/couch_db.hrl").

-define(TDEF(A), {atom_to_list(A), fun A/0}).

setup() ->
    test_util:start_couch([
            fabric
        ]).


teardown(State) ->
    test_util:stop_couch(State).


map_views_test_() ->
    {
        "Fortuna map docs",
        {
            setup,
            fun setup/0,
            fun teardown/1,
            [
                % ?TDEF(acquire_context),
                % ?TDEF(acquire_context_error),
                ?TDEF(map_docs_one)
            ]
        }
    }.

acquire_context() ->
    CtxOpts = #{
        db_name => <<"db">>,
        sig => <<"sig-1">>,
        map_funs => [
            <<"function (doc) {emit(doc._id, 1)}">>
        ]
    },
    {ok, Ctx} = fortuna:acquire_map_context(CtxOpts),
    #{
        context_id := Id
    } = Ctx,
    ?assertEqual(<<"dbsig-1">>, Id),
    ?assertEqual(fortuna:release_map_context(Ctx), ok).


acquire_context_error() ->
    CtxOpts = #{
        db_name => <<"db">>,
        sig => <<"sig-1">>,
        map_funs => [
            <<"function (doc) {emit(doc_id, ">>
        ]
    },
    {error, Error} = fortuna:acquire_map_context(CtxOpts),
    ?assertEqual(<<"Error: Line 1: Unexpected end of input">>, Error).


map_docs_one() ->
    CtxOpts = #{
        db_name => <<"db">>,
        sig => <<"sig-1">>,
        map_funs => [
            <<"function(doc) {emit(doc.val, doc.val);}">>,
            <<"function(doc) {}">>
        ]
    },
    {ok, Ctx} = fortuna:acquire_map_context(CtxOpts),
    Docs = make_docs(1),
    {ok, Result} = fortuna:map_docs(Ctx, Docs),
    ?debugFmt("resss ~p ~n", [Result]),
    ?assertEqual([{<<"1">>,[[{1,1}],[]]}], Result).


make_docs(Count) ->
    [doc(I) || I <- lists:seq(1, Count)].


doc(Id) ->
    couch_doc:from_json_obj({[
        {<<"_id">>, list_to_binary(integer_to_list(Id))},
        {<<"val">>, Id}
    ]}).