#pragma once

#include <GLFW/glfw3.h>

#ifdef __OBJC__
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#endif

class Window {
 public:
  Window();
  ~Window();

  // Initialize GLFW and create a window. Returns true on success.
  bool startup(int width = 512, int height = 512, const char* title = "Hello World");

  // Poll events helper - engine will orchestrate the loop
  bool shouldClose() const;
  void pollEvents();

  GLFWwindow* getHandle() { return handle; }
  void shutdown();

 private:
  GLFWwindow* handle = nullptr;
};
