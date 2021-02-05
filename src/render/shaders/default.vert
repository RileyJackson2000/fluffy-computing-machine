#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_pos;
out vec3 v_norm;

uniform mat4 uMVP;
uniform mat4 uM;

void main() {
    v_pos = vec4(uM * vec4(position, 1.0)).xyz;
    v_norm = mat3(uM) * normal;

    gl_Position = uMVP * vec4(position, 1.0);
}
