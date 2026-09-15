#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

layout(packed) uniform ViewData
{
  mat4 vp;
};

layout(packed) uniform InstanceData
{
  mat4 model;
};

layout(packed) uniform MaterialData
{ 
	vec4 u_tint; 
};

out vec2 Uvs;

void main() {
  gl_Position = vp * model * vec4(aPos.xyz, 1);
  Uvs = aUvs;
}