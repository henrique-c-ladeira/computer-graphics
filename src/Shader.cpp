#include "Shader.h"
#include <string>
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

Shader::Shader(const char *filename, ShaderType shaderType)
{
  char *source = readShaderSource(filename);
  if (source == NULL)
  {
    std::cerr << "Failed to read " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  shaderHandle = glCreateShader(shaderType);
  glShaderSource(shaderHandle, 1, (const GLchar **)&source, NULL);
  glCompileShader(shaderHandle);

  GLint compiled;
  glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
  if (!compiled)
  {
    std::cerr << filename << " failed to compile:" << std::endl;
    GLint logSize;
    glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logSize);
    char *logMsg = new char[logSize];
    glGetShaderInfoLog(shaderHandle, logSize, NULL, logMsg);
    std::cerr << logMsg << std::endl;
    delete[] logMsg;

    exit(EXIT_FAILURE);
  }
}

uint32_t Shader::getHandle()
{
  return shaderHandle;
}