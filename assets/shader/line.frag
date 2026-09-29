#version 460 core
in vec4 RGBA;

out vec4 FragColor;

void main()
{
	FragColor = RGBA;
	FragColor = vec4(0, 0, 0, 1);
}