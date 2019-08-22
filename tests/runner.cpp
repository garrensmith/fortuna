#define CATCH_CONFIG_RUNNER
#include "V8init.h"
#include "include/catch.hpp"

int main(int argc, char* argv[]) {
	V8Init v8init;
	v8init.initialisePlatform();

	int result = Catch::Session().run(argc, argv);

	return result;
}