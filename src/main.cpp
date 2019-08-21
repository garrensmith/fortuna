
#include "V8Init.h"
#include "environment.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << "Starting v8" << std::endl;
    V8Init v8Init;
    v8Init.initialisePlatform(argv);

    std::string id = "id1";
    Environment env(id);

    // env.hello(id);

    std::string fun = "function () { }";
    env.addFun(id, fun);
}