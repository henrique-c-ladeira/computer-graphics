#include "ShaderProgram.h"
#include <iostream>

// Create a NULL-terminated string by reading the provided file
static char *readShaderSource(const char *shaderFile)
{
  FILE *fp = fopen(shaderFile, "rb");

  if (fp == NULL)
  {
    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);

  fseek(fp, 0L, SEEK_SET);
  char *buf = new char[size + 1];
  fread(buf, 1, size, fp);

  buf[size] = '\0';
  fclose(fp);

  return buf;
}

ShaderProgram::ShaderProgram()
{
  ShaderFile vertexShader = {"shaders/vertexShader.glsl", GL_VERTEX_SHADER, NULL};
  ShaderFile fragmentShader = {"shaders/fragmentShader.glsl", GL_FRAGMENT_SHADER, NULL};
  this->shaders.push_back(vertexShader);
  this->shaders.push_back(fragmentShader);
}

ShaderProgram::ShaderProgram(const std::vector<ShaderFile> shaders)
{
  this->shaders = shaders;
}

ShaderProgram::~ShaderProgram()
{
  for (ShaderFile shaderFile : shaders)
  {
    delete[] shaderFile.source;
  }
}

ProgramHandle ShaderProgram::compile()
{
  ProgramHandle program = glCreateProgram();

  for (ShaderFile shaderFile : shaders)
  {
    shaderFile.source = readShaderSource(shaderFile.filename);
    if (shaderFile.source == NULL)
    {
      std::cerr << "Failed to read " << shaderFile.filename << std::endl;
      exit(EXIT_FAILURE);
    }

    GLuint shader = glCreateShader(shaderFile.type);
    glShaderSource(shader, 1, (const GLchar **)&shaderFile.source, NULL);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
      std::cerr << shaderFile.filename << " failed to compile:" << std::endl;
      GLint logSize;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
      char *logMsg = new char[logSize];
      glGetShaderInfoLog(shader, logSize, NULL, logMsg);
      std::cerr << logMsg << std::endl;
      delete[] logMsg;

      exit(EXIT_FAILURE);
    }

    glAttachShader(program, shader);
  }

  /* link  and error check */
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);
  if (!linked)
  {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    char *logMsg = new char[logSize];
    glGetProgramInfoLog(program, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;

    exit(EXIT_FAILURE);
  }

  return program;
}

GLint ShaderProgram::run(void *points, int sizeOfPoints)
{
  ProgramHandle program = this->compile();
  glUseProgram(program);

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
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0,
                        BUFFER_OFFSET(0));
  glClearColor(0.0, 0.0, 0.0, 1.0); // white background

  // Get the location of the rotation angle in the shader.
  GLint thetaLoc = glGetUniformLocation(program, "uTheta");
  return thetaLoc;
}
