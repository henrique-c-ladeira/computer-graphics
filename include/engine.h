#pragma once

#include "Window.h"
#include "ShaderProgram.h"
#include <vector>

class Engine
{
public:
  Engine();
  ~Engine();

  bool startup();
  void run();
  bool shutdown();

private:
  Window displayManager;
  ShaderProgram shaderProgram;
  GLint rotationAngleLocation = -1;
  float rotationAngle = 0.0f;
};