#include "ShaderProgram.h"
#include <iostream>

ShaderProgram::ShaderProgram()
{
  programHandle = glCreateProgram();
}

void ShaderProgram::compile(const std::vector<Shader> shaders)
{
  for (Shader shader : shaders)
  {
    glAttachShader(programHandle, shader.getHandle());
  }

  // link and check error
  glLinkProgram(programHandle);

  GLint linked;
  glGetProgramiv(programHandle, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint logSize;
    glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logSize);
    char *logMsg = new char[logSize];
    glGetProgramInfoLog(programHandle, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;

    exit(EXIT_FAILURE);
  }

  glUseProgram(programHandle);
}

GLint ShaderProgram::run(void *points, int sizeOfPoints)
{

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeOfPoints, points,
               GL_STATIC_DRAW);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(programHandle, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(0));

  glClearColor(0.0, 0.0, 0.0, 1.0);

  // Get the location of the rotation angle in the shader.
  GLint thetaLoc = glGetUniformLocation(programHandle, "uTheta");
  return thetaLoc;
}
