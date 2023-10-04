#pragma once
#include <OpenGL/gl3.h>
#include <vector>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

typedef GLuint ProgramHandle;

typedef struct
{
  const char *filename;
  GLenum type;
  GLchar *source;
} ShaderFile;

class ShaderProgram
{
private:
  std::vector<ShaderFile> shaders;

public:
  ShaderProgram();
  ShaderProgram(const std::vector<ShaderFile> shaders);
  ~ShaderProgram();
  ProgramHandle compile();
  GLint run(void *points, int sizeOfPoints);
};