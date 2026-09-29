#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aRGBA;

layout(packed) uniform ViewData
{
  mat4 vp;
  vec2 viewport_size;
};

layout(packed) uniform InstanceData
{
  mat4 model;
};


out vec2 Viewport_size;
out vec4 oRGBA;


void main()
{
	gl_Position = vp * model * vec4(aPos, 1.0);
	Viewport_size = viewport_size;

	oRGBA = aRGBA;
}