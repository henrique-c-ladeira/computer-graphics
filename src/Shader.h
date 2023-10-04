#pragma once
#include <OpenGL/gl3.h>

enum ShaderType
{
  fragmentShader = GL_FRAGMENT_SHADER,
  vertexShader = GL_VERTEX_SHADER
};

class Shader
{
private:
  uint32_t shaderHandle;
  ShaderType shaderType;

public:
  Shader(const char *filename, ShaderType shaderType);
};