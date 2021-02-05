#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_pos;
out vec3 v_norm;

uniform mat4 uM;
uniform mat4 uV;
uniform mat4 uP;

void main() {
    v_pos = vec4(uM * vec4(position, 1.0)).xyz;
    v_norm = (uM * vec4(normal, 0.0)).xyz;

    gl_Position = uP * uV * uM * vec4(position, 1.0);
}
