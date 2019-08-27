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

TEST_CASE("LRU", "[lru]") {
    SECTION("Can do basics") {

        LRU<std::string, int> intLru(3);

        intLru.add(std::string{"1"}, 1);
        intLru.add(std::string{"2"}, 2);
        intLru.add(std::string{"3"}, 3);

        REQUIRE(intLru.size() == 3);

        intLru.add(std::string{"3"}, 3);

        REQUIRE(intLru.size() == 3);

        REQUIRE(intLru.get(std::string{"1"}) == 1);
        REQUIRE(intLru.get(std::string{"2"}) == 2);
        REQUIRE(intLru.get(std::string{"3"}) == 3);

        intLru.add(std::string{"4"}, 4);

        REQUIRE(intLru.get(std::string{"1"}) == int());
        REQUIRE(intLru.get(std::string{"2"}) == 2);
        REQUIRE(intLru.get(std::string{"3"}) == 3);
        REQUIRE(intLru.get(std::string{"4"}) == 4);

        intLru.get(std::string{"2"});
        intLru.add(std::string{"5"}, 5);

        REQUIRE(intLru.get(std::string{"3"}) == int());
        REQUIRE(intLru.get(std::string{"2"}) == 2);
        REQUIRE(intLru.get(std::string{"4"}) == 4);
        REQUIRE(intLru.get(std::string{"5"}) == 5);

        REQUIRE(intLru.size() == 3);
    };

    SECTION("WITH ENV'S") {

        LRU<std::string, JSEnv> lru(3); 

        lru.add(std::string{"1"}, JSEnv(std::string{"1"}));
        lru.add(std::string{"2"}, JSEnv(std::string{"2"}));
        lru.add(std::string{"3"}, JSEnv(std::string{"3"}));

        auto jsenv = lru.get(std::string{"1"});

        REQUIRE(jsenv.getId() == std::string{"1"});
    }

}