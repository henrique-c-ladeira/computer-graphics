#pragma once

#include <GLFW/glfw3.h>

class Window
{
public:
  Window();
  ~Window();

  // Initialize GLFW and create a window. Returns true on success.
  bool startup(int width = 512, int height = 512, const char *title = "Hello World");

  // Poll events and swap buffers helpers — engine will orchestrate the loop.
  bool shouldClose() const;
  void pollEvents();
  void swapBuffers();
  void clear();

  GLFWwindow *getHandle() { return handle; }

  void shutdown();

private:
  GLFWwindow *handle = nullptr;
};
