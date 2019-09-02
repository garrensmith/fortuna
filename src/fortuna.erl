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

-compile(no_native).
-on_load(init/0).

-export([
    create_context/0
]).

create_context() ->
    Out = nif_create_context(),
    io:format("FROM NIF ~p ~n", [Out]).


init() ->
    io:format("LOADING ~n").
    % PrivDir = case code:priv_dir(?MODULE) of
    %     {error, _} ->
    %         EbinDir = filename:dirname(code:which(?MODULE)),
    %         AppPath = filename:dirname(EbinDir),
    %         filename:join(AppPath, "priv");
    %     Path ->
    %         Path
    % end,
    % erlang:load_nif(filename:join(PrivDir, "fortuna"), 0).


-define(NOT_LOADED, erlang:nif_error({fortuna_nif_not_loaded, ?FILE, ?LINE})).

nif_create_context() -> ?NOT_LOADED.

