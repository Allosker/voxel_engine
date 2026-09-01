#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

uniform mat4 ortho;
uniform mat4 model;

out vec2 Uvs;
out vec3 fragPos;

void main()
{
	vec4 fp = model * vec4(aPos, 1.);
	gl_Position = ortho * fp;
	fragPos = vec3(fp);

	Uvs = aUvs;
}