#import "engine.hpp"

#import <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#import <GLFW/glfw3native.h>
#import <MetalKit/MetalKit.h>

Engine::Engine() : view(nil), modelMatrix(matrix_identity_float4x4) {}

Engine::~Engine() { shutdown(); }

bool Engine::startup() {
  if (!glfwInit()) return false;

  // Tell GLFW not to create an OpenGL context
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Metal Cube", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return false;
  }

  // Get the native Cocoa window
  NSWindow* nsWindow = static_cast<NSWindow*>(glfwGetCocoaWindow(window));

  // Create a Metal-compatible view
  NSRect frame = nsWindow.contentView.frame;
  view = [[MTKView alloc] initWithFrame:frame];
  nsWindow.contentView = view;

  // Create and initialize the renderer
  renderer = std::make_unique<MetalRenderer>();
  if (!renderer->startup(view)) {
    return false;
  }

  // Initialize camera defaults
  camera.distance = 4.0f;
  camera.yaw = -45.0f;
  camera.pitch = -25.0f;
  camera.aspect = static_cast<float>(frame.size.width) / static_cast<float>(frame.size.height);

  // Register GLFW callbacks
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, Engine::cursorPosCallback);
  glfwSetMouseButtonCallback(window, Engine::mouseButtonCallback);
  glfwSetScrollCallback(window, Engine::scrollCallback);
  glfwSetKeyCallback(window, Engine::keyCallback);

  return true;
}

void Engine::run() {
  while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
    renderer->updateUniforms(camera, modelMatrix);
    glfwPollEvents();
  }
}

bool Engine::shutdown() {
  if (view) {
    [view release];
    view = nil;
  }
  renderer.reset();
  glfwTerminate();
  return true;
}

void Engine::setModelMatrix(const matrix_float4x4& m) { modelMatrix = m; }

Engine* Engine::fromWindow(GLFWwindow* wnd) {
  if (!wnd) return nullptr;
  return static_cast<Engine*>(glfwGetWindowUserPointer(wnd));
}

void Engine::cursorPosCallback(GLFWwindow* wnd, double xpos, double ypos) {
  Engine* eng = fromWindow(wnd);
  if (!eng) return;

  if (eng->leftMouseDown || eng->rightMouseDown) {
    double dx = xpos - eng->lastMouseX;
    double dy = ypos - eng->lastMouseY;
    if (eng->leftMouseDown) {
      eng->camera.orbit((float)dx, (float)-dy);
    }
    if (eng->rightMouseDown) {
      eng->camera.pan((float)dx, (float)-dy);
    }
  }

  eng->lastMouseX = xpos;
  eng->lastMouseY = ypos;
}

void Engine::mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods) {
  Engine* eng = fromWindow(wnd);
  if (!eng) return;

  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      eng->leftMouseDown = true;
      glfwGetCursorPos(wnd, &eng->lastMouseX, &eng->lastMouseY);
      glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (action == GLFW_RELEASE) {
      eng->leftMouseDown = false;
      if (!eng->rightMouseDown) glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      eng->rightMouseDown = true;
      glfwGetCursorPos(wnd, &eng->lastMouseX, &eng->lastMouseY);
      glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else if (action == GLFW_RELEASE) {
      eng->rightMouseDown = false;
      if (!eng->leftMouseDown) glfwSetInputMode(wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
  }
}

void Engine::scrollCallback(GLFWwindow* wnd, double xoffset, double yoffset) {
  Engine* eng = fromWindow(wnd);
  if (!eng) return;
  eng->camera.zoom((float)yoffset);
}

void Engine::keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
  Engine* eng = fromWindow(wnd);
  if (!eng) return;
  if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

  const float panStep = 10.0f;   // screen-space pan step
  const float orbitStep = 5.0f;  // degrees
  const float zoomStep = 0.5f;   // zoom multiplier step

  switch (key) {
    case GLFW_KEY_W:
      eng->camera.pan(0.0f, -panStep);
      break;
    case GLFW_KEY_S:
      eng->camera.pan(0.0f, panStep);
      break;
    case GLFW_KEY_A:
      eng->camera.pan(-panStep, 0.0f);
      break;
    case GLFW_KEY_D:
      eng->camera.pan(panStep, 0.0f);
      break;
    case GLFW_KEY_LEFT:
      eng->camera.orbit(-orbitStep, 0.0f);
      break;
    case GLFW_KEY_RIGHT:
      eng->camera.orbit(orbitStep, 0.0f);
      break;
    case GLFW_KEY_UP:
      eng->camera.orbit(0.0f, -orbitStep);
      break;
    case GLFW_KEY_DOWN:
      eng->camera.orbit(0.0f, orbitStep);
      break;
    case GLFW_KEY_Z:
      eng->camera.zoom(zoomStep);
      break;
    case GLFW_KEY_X:
      eng->camera.zoom(-zoomStep);
      break;
    case GLFW_KEY_SPACE:
      eng->camera = Camera();
      break;
    case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(wnd, GLFW_TRUE);
      break;
  }
}
