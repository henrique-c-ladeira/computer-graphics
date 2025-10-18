#include "engine.h";

const int NumVertices = 4;
GLfloat rotationAngle = 0.0;
GLint rotationAngleLocation;

void displayLoop()
{
  rotationAngle += 0.01;
  glUniform1f(rotationAngleLocation, rotationAngle);
  glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
}

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::init()
{
  if (!displayManager.init())
    return false;
  return true;
}

void Engine::run()
{
  displayManager.run(displayLoop);
}

bool Engine::shutdown()
{
  // Clean up and shut down the engine
}
