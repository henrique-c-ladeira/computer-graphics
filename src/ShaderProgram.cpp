#include "ShaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram() {}

ShaderProgram ShaderProgram::startup() {
  ShaderProgram shader;
  shader.programHandle = glCreateProgram();
  return shader;
}

void ShaderProgram::compile(const std::vector<Shader> shaders) {
  for (const Shader& shader : shaders) {
    glAttachShader(programHandle, shader.getHandle());
  }

  glLinkProgram(programHandle);

  GLint linked;
  glGetProgramiv(programHandle, GL_LINK_STATUS, &linked);
  if (!linked) {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint logSize;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetProgramInfoLog(programHandle, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;
    exit(EXIT_FAILURE);
  }

  glUseProgram(programHandle);
}

GLint ShaderProgram::setupGeometry(void* points, int sizeOfPoints, GLsizei count,
                                   GLint components) {
  vertexCount = count;

  // Create VAO if not exists
  if (vao == 0) glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create or update VBO
  if (vbo == 0) glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeOfPoints, points, GL_STATIC_DRAW);

  GLuint loc = glGetAttribLocation(programHandle, "vPosition");
  if ((GLint)loc >= 0) {
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, components, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
  }

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  return glGetUniformLocation(programHandle, "uTheta");
}

void ShaderProgram::draw() {
  if (programHandle == 0) return;

  glUseProgram(programHandle);
  if (vao != 0) glBindVertexArray(vao);

  if (vertexCount > 0) glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
}

void ShaderProgram::setUniform1f(const char* name, float value) {
  GLint loc = glGetUniformLocation(programHandle, name);
  if (loc >= 0) glUniform1f(loc, value);
}

void ShaderProgram::setUniformMat4(const char* name, const float* matrix) {
  GLint loc = glGetUniformLocation(programHandle, name);
  if (loc >= 0) glUniformMatrix4fv(loc, 1, GL_FALSE, matrix);
}