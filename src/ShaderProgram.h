#pragma once
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#include <vector>
#include <glm/vec2.hpp>

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