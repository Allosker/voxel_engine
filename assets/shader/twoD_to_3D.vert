#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

out vec2 Uvs;

uniform mat4 vp;
uniform mat4 model;


void main()
{
	gl_Position = vp * model * vec4(aPos, 1.);
	Uvs = aUvs;
}