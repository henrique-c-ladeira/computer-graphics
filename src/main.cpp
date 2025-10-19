#include <iostream>
#include "engine.h"
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Shader.h"
#include "ShaderProgram.h"

ShaderProgram shaderProgram;

void displayLoop()
{
  shaderProgram.draw();
}

int main(int argc, char **argv)
{

  Engine engine = Engine();
  if (!engine.startup())
  {
    std::cerr << "Failed to initialize engine" << std::endl;
    return -1;
  }

  engine.run();

  return 0;
}
