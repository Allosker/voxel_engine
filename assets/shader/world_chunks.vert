#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUvs;
layout (location = 2) in float aAo;

layout(packed) uniform ViewData
{
  mat4 vp;
};

layout(packed) uniform InstanceData
{
  mat4 model;
};

out vec2 Uvs;
out float AO;

void main()
{
	gl_Position = vp * model * vec4(aPos.xyz, 1);
	Uvs = aUvs;
	AO = aAo;
}