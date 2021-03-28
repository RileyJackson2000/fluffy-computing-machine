#version 330 core

#ifndef MAX_NUM_DIR_LIGHTS
#define MAX_NUM_DIR_LIGHTS 2
#endif

#ifndef MAX_NUM_POINT_LIGHTS
#define MAX_NUM_POINT_LIGHTS 5
#endif

#ifndef MAX_NUM_SHADOWS
#define MAX_NUM_SHADOWS 4
#endif

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

out vec3 v_pos; // position in world space
out vec3 v_norm;
out vec2 v_uv; // texture coord

uniform mat4 uMVP;
uniform mat4 uM;
uniform mat3 uMti;

void main() {
    vec4 v_pos4 = uM * vec4(position, 1.f);
    v_pos = vec3(v_pos4) / v_pos4.w;
    v_norm = normalize(uMti * normal);
    v_uv = uv;

    gl_Position = uMVP * vec4(position, 1.f);
}
