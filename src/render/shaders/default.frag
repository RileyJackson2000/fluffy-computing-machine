#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_pos;
in vec3 v_norm;

uniform vec4 uColor;
uniform vec3 uLightPos;

void main()
{
	vec3 dir = normalize(v_pos - uLightPos);
	float diffuse = clamp(dot(dir, v_norm), 0, 1);
	color = diffuse * uColor;
}
