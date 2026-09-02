#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

uniform mat4 ortho;
uniform mat4 model;

out vec2 Uvs;
out vec3 fragPos;

void main()
{
	gl_Position = ortho *  model * vec4(aPos, 1.);
	fragPos = vec3(aPos); // Directly pass in local pos so that the lighting stays the same

	Uvs = aUvs;
}