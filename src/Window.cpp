#include "Window.h"

void Window::init()
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
};

void Window::display()
{
  window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    // render.draw();
    // glClear(GL_COLOR_BUFFER_BIT);
    // glUniform1f(rotationAngleLocation, rotationAngle);
    // glDrawArrays(GL_TRIANGLES, 0, NumTriangles);
    // glFlush();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }
};

// Window::~Window()
// {
//   // glfwTerminate();
// }