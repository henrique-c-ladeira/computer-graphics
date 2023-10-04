#version 150

in vec4 vPosition;

uniform float uTheta;

out vec4 color;

void main() {
  gl_Position = vec4(vPosition.xy * mat2(cos(uTheta),-sin(uTheta), 
                                          sin(uTheta), cos(uTheta)),
                    vPosition.zw);
  color = vec4((1.0 + vPosition.xyz) / 2.0, 1.0);
}