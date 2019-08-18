
#include "V8Init.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "Hello" << std::endl;
  V8Init v8Init;
  v8Init.initialisePlatform(argv);
}