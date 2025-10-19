#pragma once

#include <vector>

#include "Camera.h"
#include "ShaderProgram.h"
#include "Window.h"

class Engine {
 public:
  Engine();
  ~Engine();

  bool startup();
  void run();
  bool shutdown();

 private:
  Window displayManager;
  ShaderProgram shaderProgram;
  Camera camera;

  // No automatic rotation: render the model matrices provided by the caller or use identity.

  // Input state
  bool leftMouseDown = false;
  bool rightMouseDown = false;
  double lastMouseX = 0.0, lastMouseY = 0.0;

  // Callbacks
  static void cursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods);
  static void scrollCallback(GLFWwindow* wnd, double xoffset, double yoffset);

  // Helper to retrieve the Engine instance from GLFW window user pointer
  static Engine* fromWindow(GLFWwindow* wnd);
};