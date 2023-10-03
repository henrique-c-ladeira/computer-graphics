#pragma once
#include <string>

enum ShaderType
{
  fragmentShader,
  vertexShader
};

class Shader
{
private:
  const std::string fileName;
  const ShaderType shaderType;

public:
  Shader(std::string fileName, ShaderType shaderType) : fileName(fileName), shaderType(shaderType){};
};