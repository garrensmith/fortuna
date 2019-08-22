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
#include "jsenv.h"

TEST_CASE("JSEnv tests", "[jsenv]") {
	SECTION("Can map doc") {
		JSEnv env(std::string("context-id"));

		auto id = std::string("id-1");
		auto fun = std::string("function (doc) {emit(doc._id, 1)}");
		env.addFun(id, fun);

		auto doc = std::string(R"({"_id":"my-id","value":123})");
		REQUIRE(env.mapDoc(doc) == R"([{"id":"id-1","result":[["my-id",1]]}])");
		REQUIRE(env.mapDoc(doc) == R"([{"id":"id-1","result":[["my-id",1]]}])");

		auto doc2 = std::string(R"({"_id":"my-id-2","value":567})");
		REQUIRE(env.mapDoc(doc2) == R"([{"id":"id-1","result":[["my-id-2",1]]}])");
		REQUIRE(env.mapDoc(doc2) == R"([{"id":"id-1","result":[["my-id-2",1]]}])");
	};

	SECTION("Create multiple environemnts to map with") {
		auto id = std::string("id-1");
		auto fun = std::string("function (doc) {emit(doc._id, 1)}");
		auto doc = std::string(R"({"_id":"my-id","value":123})");

		JSEnv env1(std::string("context-id-1"));
		JSEnv env2(std::string("context-id-2"));
		JSEnv env3(std::string("context-id-3"));

		env1.addFun(id, fun);
		env2.addFun(id, fun);
		env3.addFun(id, fun);

		REQUIRE(env1.mapDoc(doc) == R"([{"id":"id-1","result":[["my-id",1]]}])");
		REQUIRE(env2.mapDoc(doc) == R"([{"id":"id-1","result":[["my-id",1]]}])");
		REQUIRE(env3.mapDoc(doc) == R"([{"id":"id-1","result":[["my-id",1]]}])");
	}
}