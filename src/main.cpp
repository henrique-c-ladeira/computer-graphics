#include <iostream>
#include "engine.h"

int main(int argc, char **argv)
{

  Engine engine = Engine();
  if (!engine.init())
  {
    std::cerr << "Failed to initialize engine" << std::endl;
    return -1;
  }
  engine.run();

  return 0;
}
