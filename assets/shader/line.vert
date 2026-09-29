#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aRGBA;

layout(packed) uniform ViewData
{
  mat4 vp;
};

layout(packed) uniform InstanceData
{
  mat4 model;
};


out vec4 oRGBA;


void main()
{
	gl_Position = vp * model * vec4(aPos, 1.0);

	oRGBA = aRGBA;
}