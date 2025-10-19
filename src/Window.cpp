#include "Window.h"

#include <iostream>

Window::Window() {}

bool Window::startup(int width, int height, const char* title) {
  if (!glfwInit()) return false;

  // Tell GLFW not to create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  handle = glfwCreateWindow(width, height, title, NULL, NULL);
  if (!handle) {
    glfwTerminate();
    return false;
  }

  return true;
}

bool Window::shouldClose() const { return glfwWindowShouldClose(handle); }

void Window::pollEvents() { glfwPollEvents(); }

void Window::shutdown() {
  if (handle) {
    glfwDestroyWindow(handle);
    handle = nullptr;
  }
  glfwTerminate();
}

Window::~Window() { shutdown(); }