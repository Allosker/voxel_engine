#version 460 core

in vec2 Uvs;
in float AO;

uniform sampler2D tex;

out vec4 FragColor;

void main()
{
	FragColor = texture(tex, Uvs) * AO;
}