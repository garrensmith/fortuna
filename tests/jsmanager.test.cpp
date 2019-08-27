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

#include "catch.hpp"
#include "jsenv_manager.h"

TEST_CASE("JSEnvManager tests", "[jsenv]") {
	SECTION("Can create and get") {
		JSEnvManager manager;
		auto id = std::string("id-1");
		auto env1 = manager.createEnv(id);

		auto env2 = manager.get(id);
		auto env3 = manager.createEnv(std::string("id-2"));

		REQUIRE(env1 == env2);
		REQUIRE(env1 != env3);
	};

	SECTION("Can getOrCreateEnv") {
		JSEnvManager manager;
		auto id = std::string("id-1");
		auto env1 = manager.getOrCreateEnv(id);

		auto env2 = manager.getOrCreateEnv(id);

		REQUIRE(env1 == env2);
	}

	SECTION("Can delete") {
		JSEnvManager manager;
		auto id = std::string("id-1");
		auto env1 = manager.getOrCreateEnv(id);
		manager.deleteEnv(id);

		REQUIRE(manager.hasEnv(id) == false);
	}
}