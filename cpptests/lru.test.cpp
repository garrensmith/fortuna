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
#include "lru.h"
#include "jsenv.h"

using namespace std;

TEST_CASE("LRU", "[lru]") {
	SECTION("Can do basics") {

		LRU<string, int> intLru(3);

		intLru.add(string{ "1" }, 1);
		intLru.add(string{ "2" }, 2);
		intLru.add(string{ "3" }, 3);

		REQUIRE(intLru.size() == 3);

		intLru.add(string{ "3" }, 3);

		REQUIRE(intLru.size() == 3);

		REQUIRE(intLru.get(string{ "1" }) == 1);
		REQUIRE(intLru.get(string{ "2" }) == 2);
		REQUIRE(intLru.get(string{ "3" }) == 3);

		intLru.add(string{ "4" }, 4);

		REQUIRE(intLru.get(string{ "1" }) == int());
		REQUIRE(intLru.get(string{ "2" }) == 2);
		REQUIRE(intLru.get(string{ "3" }) == 3);
		REQUIRE(intLru.get(string{ "4" }) == 4);

		intLru.get(string{ "2" });
		intLru.add(string{ "5" }, 5);

		REQUIRE(intLru.get(string{ "3" }) == int());
		REQUIRE(intLru.get(string{ "2" }) == 2);
		REQUIRE(intLru.get(string{ "4" }) == 4);
		REQUIRE(intLru.get(string{ "5" }) == 5);

		REQUIRE(intLru.size() == 3);
	};

	SECTION("WITH JSEnv") {

		LRU<string, shared_ptr<JSEnv>> lru(3);
		auto js1 = make_shared<JSEnv>(string{ "1" });
		lru.add(string{ "1" }, js1);

		auto jsenv = lru.get(string{ "1" });

		REQUIRE(jsenv->getId() == string{ "1" });

		auto jsenv1 = lru.get(string{ "2" });
		REQUIRE(jsenv1 == shared_ptr<JSEnv>());
	}

	SECTION("Erase") {

		LRU<string, int> intLru(3);
		auto id = string{ "1" };
		intLru.add(id, 1);

		intLru.erase(id);
		REQUIRE(intLru.get(id) == int());
	}
}