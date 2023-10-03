#include "ShaderProgram.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

//----------------------------------------------------------------------------

const int NumTimesToSubdivide = 5;
const int NumTriangles = 729; // 3^5 triangles generated
const int NumVertices = 3 * NumTriangles;
glm::vec2 points[NumVertices];
int Index = 0;
GLfloat rotationAngle = 0.0;
GLint rotationAngleLocation;

void triangle(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c)
{
  points[Index++] = a;
  points[Index++] = b;
  points[Index++] = c;
}

void divide_triangle(const glm::vec2 &a, const glm::vec2 &b, const glm::vec2 &c, int count)
{
  if (count > 0)
  {
    // compute midpoints of sides
    glm::vec2 v0 = (a + b) / 2.0f;
    glm::vec2 v1 = (a + c) / 2.0f;
    glm::vec2 v2 = (b + c) / 2.0f;
    // subdivide all but middle triangle
    divide_triangle(a, v0, v1, count - 1);
    divide_triangle(c, v1, v2, count - 1);
    divide_triangle(b, v2, v0, count - 1);
  }
  else
  {
    triangle(a, b, c); // draw triangle at end of recursion
  }
}

void init(void)
{
  glm::vec2 vertices[3] = {
      glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, -1.0)};
  // Subdivide the original triangle
  divide_triangle(vertices[0], vertices[1], vertices[2],
                  NumTimesToSubdivide);

  // Load shaders and use the resulting shader program
  ShaderProgram shaderProgram = ShaderProgram();
  rotationAngleLocation = shaderProgram.run(points, sizeof(points));
}

int main(int argc, char **argv)
{
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  init();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    rotationAngle += 0.01;
    glUniform1f(rotationAngleLocation, rotationAngle);
    glDrawArrays(GL_TRIANGLES, 0, NumTriangles);
    glFlush();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
