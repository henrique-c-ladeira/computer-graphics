#version 150

in vec3 vWorldPos;
out vec4 fColour;

void main()
{
  vec3 col = (normalize(vWorldPos) * 0.5) + vec3(0.5);
  fColour = vec4(col, 1.0);
}