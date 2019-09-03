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

#include "V8init.h"
#include "jsenv.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
	std::cout << "Starting v8" << std::endl;
	V8Init v8Init;
	v8Init.initialisePlatform();

	std::string id = "id1";
	JSEnv env(id);

	auto fun = std::string("function (doc) {emit(doc._id, 1)}");
	env.addFun(id, fun);

	auto doc = std::string(R"({"_id":"my-id","value":123})");
	auto out = env.mapDoc(doc);
}