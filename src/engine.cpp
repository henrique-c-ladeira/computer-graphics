#include "engine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Shader.h"

// Cube: 36 vertices (12 triangles) with 3 components each
static const GLfloat cubeVertices[] = {
    // front face
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    // back face
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    // left face
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    // right face
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    // top face
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    0.5f,
    -0.5f,
    // bottom face
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    -0.5f,
    0.5f,
    -0.5f,
    0.5f,
};

static const GLsizei CubeVertexCount = sizeof(cubeVertices) / (sizeof(GLfloat) * 3);

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::startup() {
  if (!displayManager.startup(800, 600, "SimpleCube")) return false;

  std::vector<Shader> shaders = Shader::createShaders();

  shaderProgram = ShaderProgram::startup();
  shaderProgram.compile(shaders);

  // Upload cube geometry (3 components per vertex)
  // We don't need a rotation uniform location anymore; just set up geometry.
  (void)shaderProgram.setupGeometry((void*)cubeVertices, sizeof(cubeVertices), CubeVertexCount, 3);

  // Initialize camera defaults
  camera.distance = 4.0f;
  camera.yaw = -45.0f;
  camera.pitch = -25.0f;

  // Register GLFW callbacks and set user pointer so static callbacks can retrieve Engine*
  GLFWwindow* wnd = displayManager.getHandle();
  glfwSetWindowUserPointer(wnd, this);
  glfwSetCursorPosCallback(wnd, Engine::cursorPosCallback);
  glfwSetMouseButtonCallback(wnd, Engine::mouseButtonCallback);
  glfwSetScrollCallback(wnd, Engine::scrollCallback);

  return true;
}

void Engine::run() {
  while (!displayManager.shouldClose()) {
    displayManager.clear();

    // No autorotation: use identity model matrix so the renderer shows whatever the model provides
    glm::mat4 model = glm::mat4(1.0f);

    // Get current framebuffer size for correct aspect ratio
    int fbw = 800, fbh = 600;
    GLFWwindow* wnd = displayManager.getHandle();
    glfwGetFramebufferSize(wnd, &fbw, &fbh);
    float aspect = (fbh == 0) ? 1.0f : (float)fbw / (float)fbh;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    // Use camera for view matrix
    glm::mat4 view = camera.getViewMatrix();

    // upload uniforms
    shaderProgram.setUniformMat4("uModel", glm::value_ptr(model));
    shaderProgram.setUniformMat4("uView", glm::value_ptr(view));
    shaderProgram.setUniformMat4("uProjection", glm::value_ptr(projection));

    shaderProgram.draw();

    displayManager.swapBuffers();
    displayManager.pollEvents();
  }
}

bool Engine::shutdown() {
  displayManager.shutdown();
  return true;
}

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
      // orbit: horizontal -> yaw, vertical -> pitch (invert Y for intuitive control)
      eng->camera.orbit((float)dx, (float)-dy);
    }
    if (eng->rightMouseDown) {
      // pan
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
    } else if (action == GLFW_RELEASE) {
      eng->leftMouseDown = false;
    }
  }

  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      eng->rightMouseDown = true;
      glfwGetCursorPos(wnd, &eng->lastMouseX, &eng->lastMouseY);
    } else if (action == GLFW_RELEASE) {
      eng->rightMouseDown = false;
    }
  }
}

void Engine::scrollCallback(GLFWwindow* wnd, double xoffset, double yoffset) {
  Engine* eng = fromWindow(wnd);
  if (!eng) return;
  eng->camera.zoom((float)yoffset);
}
