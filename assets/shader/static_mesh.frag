#version 460 core

in vec2 Uvs;

uniform sampler2D tex;

layout(std140) uniform MaterialData
{
    vec4 u_tint;
};

out vec4 FragColor;

void main()
{
	FragColor = texture(tex, Uvs) * u_tint;
}