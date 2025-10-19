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
  rotationAngleLocation =
      shaderProgram.setupGeometry((void*)cubeVertices, sizeof(cubeVertices), CubeVertexCount, 3);

  return true;
}

void Engine::run() {
  // Camera setup
  glm::vec3 camPos(2.0f, 2.0f, 2.0f);
  glm::vec3 camTarget(0.0f, 0.0f, 0.0f);
  glm::vec3 camUp(0.0f, 1.0f, 0.0f);

  float aspect = 800.0f / 600.0f;
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

  while (!displayManager.shouldClose()) {
    displayManager.clear();

    // update model (rotation)
    rotationAngle += 0.01f;
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(camPos, camTarget, camUp);

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
