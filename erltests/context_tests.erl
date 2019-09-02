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

acquire_context_test() ->
    CtxOpts = #{
        db_name => <<"db">>,
        sig => <<"sig-1">>,
        map_funs => [
            "function (doc) {emit(doc._id, 1)"
        ]
    },
    {ok, Ctx} = fortuna:acquire_map_context(CtxOpts),
    ?debugFmt("OUT ~p ~n", [Ctx]),
    #{
        context_id := Id
    } = Ctx,
    ?debugFmt("ID ~p ~n", [Id]),
    ?assertEqual(<<"dbsig-1">>, Id).