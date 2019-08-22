
#include "V8init.h"
#include "jsenv.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]) {
	std::cout << "Starting v8" << std::endl;
	V8Init v8Init;
	v8Init.initialisePlatform(argv);

	std::string id = "id1";
	JSEnv env(id);

	// env.hello(id);

	// std::string fun = "function () { }";
	auto fun = std::string("function (doc) {emit(doc._id, 1)}");
	env.addFun(id, fun);

	auto doc = std::string(R"({"_id":"my-id","value":123})");
	auto out = env.mapDoc(doc);
}