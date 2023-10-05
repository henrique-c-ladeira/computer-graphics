#include "Shader.h"
#include "ShaderProgram.h"
#include "Window.h"
#include "shaderUtils.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

const int NumVertices = 4;

GLfloat rotationAngle = 0.0;
GLint rotationAngleLocation;

void displayLoop()
{
  rotationAngle += 0.01;
  glUniform1f(rotationAngleLocation, rotationAngle);
  glDrawArrays(GL_TRIANGLE_FAN, 0, NumVertices);
}

int main(int argc, char **argv)
{
  glm::vec2 vertices[NumVertices] = {
      glm::vec2(-0.5, -0.5),
      glm::vec2(0.5, -0.5),
      glm::vec2(0.5, 0.5),
      glm::vec2(-0.5, 0.5),
  };

  Window window = Window();

  std::vector<Shader> shaders = createShaders();

  ShaderProgram shaderProgram = ShaderProgram();

  shaderProgram.compile(shaders);
  rotationAngleLocation = shaderProgram.run(vertices, sizeof(vertices));

  window.display(displayLoop);

  return 0;
}
