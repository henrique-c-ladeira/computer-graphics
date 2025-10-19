#pragma once

#include <simd/simd.h>

// Matrix math helpers
inline matrix_float4x4 matrix_look_at_right_hand(vector_float3 eye, vector_float3 target,
                                                 vector_float3 up) {
  vector_float3 z = vector_normalize(eye - target);
  vector_float3 x = vector_normalize(vector_cross(up, z));
  vector_float3 y = vector_cross(z, x);

  matrix_float4x4 m = {
      .columns[0] = {x.x, y.x, z.x, 0.0f},
      .columns[1] = {x.y, y.y, z.y, 0.0f},
      .columns[2] = {x.z, y.z, z.z, 0.0f},
      .columns[3] = {-vector_dot(x, eye), -vector_dot(y, eye), -vector_dot(z, eye), 1.0f}};

  return m;
}

inline matrix_float4x4 matrix_perspective_right_hand(float fovRadians, float aspect, float nearZ,
                                                     float farZ) {
  float ys = 1 / tanf(fovRadians * 0.5);
  float xs = ys / aspect;
  float zs = farZ / (nearZ - farZ);

  matrix_float4x4 m = {.columns[0] = {xs, 0.0f, 0.0f, 0.0f},
                       .columns[1] = {0.0f, ys, 0.0f, 0.0f},
                       .columns[2] = {0.0f, 0.0f, zs, -1.0f},
                       .columns[3] = {0.0f, 0.0f, nearZ * zs, 0.0f}};

  return m;
}

class Camera {
 public:
  simd::float3 target{0.0f, 0.0f, 0.0f};
  float distance = 3.0f;
  float yaw = -45.0f;    // degrees
  float pitch = -30.0f;  // degrees
  float fov = 45.0f;
  float aspect = 1.0f;
  float nearZ = 0.1f;
  float farZ = 100.0f;

  float minDistance = 0.5f;
  float maxDistance = 20.0f;

  Camera() = default;

  simd::float3 position() const {
    float radYaw = yaw * M_PI / 180.0f;
    float radPitch = pitch * M_PI / 180.0f;
    float x = distance * cos(radPitch) * cos(radYaw);
    float y = distance * sin(radPitch);
    float z = distance * cos(radPitch) * sin(radYaw);
    return target + simd::float3{x, y, z};
  }

  matrix_float4x4 getViewMatrix() const {
    simd::float3 pos = position();
    simd::float3 up{0.0f, 1.0f, 0.0f};
    return matrix_look_at_right_hand(pos, target, up);
  }

  matrix_float4x4 getProjectionMatrix() const {
    return matrix_perspective_right_hand(fov * M_PI / 180.0f, aspect, nearZ, farZ);
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
    simd::float3 pos = position();
    simd::float3 dir = vector_normalize(target - pos);
    simd::float3 right = vector_normalize(vector_cross(simd::float3{0, 1, 0}, dir));
    simd::float3 up = vector_cross(dir, right);

    target += (-right * deltaX + up * deltaY) * sensitivity * distance;
  }
};
