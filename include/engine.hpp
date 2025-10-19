#pragma once

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3.h>
#import <GLFW/glfw3native.h>
@class MTKView;
#else
#include <GLFW/glfw3.h>
class MTKView;  // Forward declaration for non-ObjC code
#endif

#include <memory>

#include "Camera.h"
#include "MetalRenderer.h"

#ifdef __OBJC__
class Engine {
#else
class Engine {
#endif
 public:
  Engine();
  ~Engine();

  bool startup();
  void run();
  bool shutdown();
  void setModelMatrix(const matrix_float4x4& m);

 private:
#ifdef __OBJC__
  MTKView* view;
#else
  void* view;  // Opaque pointer for non-ObjC code
#endif
  std::unique_ptr<MetalRenderer> renderer;
  Camera camera;
  matrix_float4x4 modelMatrix;

  // Input state
  bool leftMouseDown = false;
  bool rightMouseDown = false;
  double lastMouseX = 0.0, lastMouseY = 0.0;

  // Callbacks
  static void cursorPosCallback(GLFWwindow* wnd, double xpos, double ypos);
  static void mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods);
  static void scrollCallback(GLFWwindow* wnd, double xoffset, double yoffset);
  static void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);

  static Engine* fromWindow(GLFWwindow* wnd);
};