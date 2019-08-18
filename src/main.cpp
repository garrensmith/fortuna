
#include <cstdlib>
#include <iostream>
#include "V8Init.h"

int main (int argc, char *argv[]) { 
    std::cout << "Hello" << std::endl;
    V8Init v8Init;
    v8Init.initialisePlatform(argv);
} 