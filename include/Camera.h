#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
 public:
  glm::vec3 target{0.0f, 0.0f, 0.0f};
  float distance = 3.0f;
  float yaw = -45.0f;    // degrees
  float pitch = -30.0f;  // degrees

  float minDistance = 0.5f;
  float maxDistance = 20.0f;

  Camera() = default;

  glm::vec3 position() const {
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    float x = distance * cos(radPitch) * cos(radYaw);
    float y = distance * sin(radPitch);
    float z = distance * cos(radPitch) * sin(radYaw);
    return target + glm::vec3(x, y, z);
  }

  glm::mat4 getViewMatrix() const {
    return glm::lookAt(position(), target, glm::vec3(0.0f, 1.0f, 0.0f));
  }

  void orbit(float deltaX, float deltaY, float sensitivity = 0.2f) {
    yaw += deltaX * sensitivity;
    pitch += deltaY * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
  }

  void zoom(float delta, float sensitivity = 0.1f) {
    distance *= (1.0f - delta * sensitivity);
    if (distance < minDistance) distance = minDistance;
    if (distance > maxDistance) distance = maxDistance;
  }

  void pan(float deltaX, float deltaY, float sensitivity = 0.005f) {
    // Move target in camera right and up directions
    glm::vec3 dir = glm::normalize(target - position());
    glm::vec3 right = glm::normalize(glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, dir));
    target += -right * deltaX * sensitivity * distance + up * deltaY * sensitivity * distance;
  }
};
