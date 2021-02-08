#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_pos;
out vec3 v_norm;

uniform mat4 uMVP;
uniform mat4 uM;
uniform mat3 uMti;

void main() {
    vec4 v_pos4 = uM * vec4(position, 1.f);
    v_pos = vec3(v_pos4) / v_pos4.w;
    v_norm = normalize(uMti * normal);

    gl_Position = uMVP * vec4(position, 1.f);
}
