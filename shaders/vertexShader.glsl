#version 150

in vec3 vPosition;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

out vec3 vWorldPos;

void main()
{
  vec4 worldPos = uModel * vec4(vPosition, 1.0);
  gl_Position = uProjection * uView * worldPos;
  vWorldPos = worldPos.xyz;
}