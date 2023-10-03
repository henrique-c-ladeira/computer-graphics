#version 150

in vec4 vPosition;

uniform float uTheta;

out vec4 color;

void main() {
  gl_Position = vPosition;
  color = vec4((1.0 + vPosition.xyz) / 2.0, 1.0);
}