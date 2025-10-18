#pragma once
#include "Shader.h"
#include <OpenGL/gl3.h>
#include <vector>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

typedef uint32_t ProgramHandle;
class ShaderProgram
{
private:
  ProgramHandle programHandle;

public:
  ShaderProgram();
  void compile(const std::vector<Shader> shaders);
  GLint run(void *points, int sizeOfPoints);
};