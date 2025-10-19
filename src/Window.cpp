#include "Window.h"
#include <iostream>

Window::Window() {}

bool Window::startup(int width, int height, const char *title)
{
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  handle = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!handle)
  {
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(handle);
  return true;
}

bool Window::shouldClose() const
{
  return glfwWindowShouldClose(handle);
}

void Window::pollEvents()
{
  glfwPollEvents();
}

void Window::swapBuffers()
{
  glfwSwapBuffers(handle);
}

void Window::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}

void Window::shutdown()
{
  if (handle)
  {
    glfwDestroyWindow(handle);
    handle = nullptr;
  }
  glfwTerminate();
}

Window::~Window()
{
  shutdown();
}