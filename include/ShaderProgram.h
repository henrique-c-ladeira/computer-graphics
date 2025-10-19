#pragma once
#include <OpenGL/gl3.h>

#include <vector>

#include "Shader.h"

#define BUFFER_OFFSET(offset) ((GLvoid*)(offset))

typedef uint32_t ProgramHandle;
class ShaderProgram {
 private:
  ProgramHandle programHandle;
  GLuint vao = 0;
  GLuint vbo = 0;
  GLsizei vertexCount = 0;

 public:
  ShaderProgram();
  static ShaderProgram startup();
  void compile(const std::vector<Shader> shaders);

  // Upload vertex data and return a uniform location if needed
  GLint setupGeometry(void* points, int sizeOfPoints, GLsizei count, GLint components = 2);

  // Draw the uploaded geometry
  void draw();

  // Convenience uniform setters
  void setUniform1f(const char* name, float value);
  void setUniformMat4(const char* name, const float* matrix);
};