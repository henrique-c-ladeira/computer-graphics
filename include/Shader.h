#pragma once
#include <OpenGL/gl3.h>
#include <vector>

enum ShaderType
{
  fragmentShaderType = GL_FRAGMENT_SHADER,
  vertexShaderType = GL_VERTEX_SHADER
};

class Shader
{
private:
  uint32_t shaderHandle;
  ShaderType shaderType;

public:
  static std::vector<Shader> createShaders();
  Shader(const char *filename, ShaderType shaderType);
  uint32_t getHandle();
};