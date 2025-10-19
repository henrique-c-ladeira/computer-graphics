#include <iostream>
#include "engine.h"

// int main(int argc, char **argv)
// {

//   Engine engine = Engine();
//   if (!engine.init())
//   {
//     std::cerr << "Failed to initialize engine" << std::endl;
//     return -1;
//   }
//   engine.run();
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include "Window.h"
#include "Shader.h"
#include "ShaderProgram.h"

ShaderProgram shaderProgram;
Window *window;

void displayLoop()
{
  shaderProgram.draw();
}

int main(int argc, char **argv)
{
  glm::vec2 vertices[] = {
      glm::vec2(-0.5, -0.5),
      glm::vec2(0.5, -0.5),
      glm::vec2(0.5, 0.5),
      glm::vec2(-0.5, 0.5),
  };

  window = Window::startup();

  std::vector<Shader> shaders = Shader::createShaders();

  shaderProgram = ShaderProgram::startup();

  shaderProgram.compile(shaders);
  shaderProgram.run(vertices, sizeof(vertices));

  window->display(displayLoop);

  return 0;
}
