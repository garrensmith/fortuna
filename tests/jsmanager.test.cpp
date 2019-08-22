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
        REQUIRE_THROWS(manager.get(id));
    }
}