#include "Window.h"

Window::Window()
{
}

Window *Window::startup()
{
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  Window *window = new Window();
  window->handle = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
  if (!window->handle)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window->handle);
  return window;
};

void Window::display(void (*loop)())
{
  while (!glfwWindowShouldClose(handle))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    loop();

    glFlush();
    /* Swap front and back buffers */
    glfwSwapBuffers(handle);

    /* Poll for and process events */
    glfwPollEvents();
  }
};

Window::~Window()
{
  glfwTerminate();
}