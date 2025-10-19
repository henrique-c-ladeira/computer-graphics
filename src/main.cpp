#include <iostream>

#include "engine.h"

int main(int argc, char** argv) {
  Engine engine;
  if (!engine.startup()) {
    std::cerr << "Failed to initialize engine" << std::endl;
    return -1;
  }

  engine.run();

  engine.shutdown();
  return 0;
}
