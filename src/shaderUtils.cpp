#include "shaderUtils.h"

std::vector<Shader> createShaders()
{
  Shader vertexShader = Shader("shaders/vertexShader.glsl", vertexShaderType);
  Shader fragmentShader = Shader("shaders/fragmentShader.glsl", fragmentShaderType);

  std::vector<Shader> shaders;
  shaders.push_back(vertexShader);
  shaders.push_back(fragmentShader);

  return shaders;
}