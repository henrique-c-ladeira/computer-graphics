#include "engine.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include <glm/vec2.hpp>

// const int NumVertices = 4;
// GLfloat rotationAngle = 0.0;
// GLint rotationAngleLocation;

// void displayLoop()
// {
//   rotationAngle += 0.01;
//   glUniform1f(rotationAngleLocation, rotationAngle);
//   glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
// }

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::startup()
{
  displayManager = Window();
  if (!displayManager.startup())
    return false;
  return true;

  glm::vec2 vertices[] = {
      glm::vec2(-0.5, -0.5),
      glm::vec2(0.5, -0.5),
      glm::vec2(0.5, 0.5),
      glm::vec2(-0.5, 0.5),
  };

  std::vector<Shader> shaders = Shader::createShaders();

  ShaderProgram shaderProgram = ShaderProgram::startup();

  shaderProgram.compile(shaders);
  shaderProgram.run(vertices, sizeof(vertices));
}

void displayLoop()
{
  shaderProgram.draw();
}

void Engine::run()
{

  displayManager.display(displayLoop);
}

bool Engine::shutdown()
{
  // Clean up and shut down the engine
}
