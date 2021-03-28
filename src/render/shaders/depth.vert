#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 uMVP;
uniform mat4 uVP;
uniform mat4 uM;

void main() {
  gl_Position = uMVP * vec4(position, 1.0);
  //gl_Position = uVP * uM * vec4(position, 1.0);
}
