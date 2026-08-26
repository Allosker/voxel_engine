#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUvs;

out vec2 Uvs;

uniform mat4 ortho;
uniform mat4 model;


void main()
{
	gl_Position = ortho * model * vec4(aPos.xy, 0., 1.);
	Uvs = aUvs;
}