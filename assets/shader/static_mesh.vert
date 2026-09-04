#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUvs;

layout(std140) uniform SceneData
{
    mat4 model;
    mat4 vp;
};

layout(std140) uniform MaterialData
{
    vec4 u_tint;
};

out vec2 Uvs;

void main()
{
	gl_Position = vp * model * vec4(aPos.xyz, 1);
	Uvs = aUvs;
}