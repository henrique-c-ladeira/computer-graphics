#include "engine.h"
#include "Shader.h"
#include <glm/vec2.hpp>
#include <iostream>

static const int NumVertices = 4;

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::startup()
{
  if (!displayManager.startup(512, 512, "Hello World"))
    return false;

  // Setup geometry and shaders (moved here from main.cpp)
  glm::vec2 vertices[NumVertices] = {
      glm::vec2(-0.5f, -0.5f),
      glm::vec2(0.5f, -0.5f),
      glm::vec2(0.5f, 0.5f),
      glm::vec2(-0.5f, 0.5f),
  };

  std::vector<Shader> shaders = Shader::createShaders();

  shaderProgram = ShaderProgram::startup();
  shaderProgram.compile(shaders);
  rotationAngleLocation = shaderProgram.run(vertices, sizeof(vertices));

  return true;
}

void Engine::run()
{
  while (!displayManager.shouldClose())
  {
    displayManager.clear();

    // Update rotation + shader uniforms + draw
    rotationAngle += 0.01f;
    if (rotationAngleLocation >= 0)
      glUniform1f(rotationAngleLocation, rotationAngle);

    shaderProgram.draw();

    displayManager.swapBuffers();
    displayManager.pollEvents();
  }
}

bool Engine::shutdown()
{
  displayManager.shutdown();
  return true;
}
