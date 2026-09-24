#version 460 core
layout(location = 0) in vec3 aPos_a;
layout(location = 1) in vec3 aPos_b;
layout(location = 2) in vec4 aRGBA;

layout(packed) uniform ViewData
{
  mat4 vp;
};

layout(packed) uniform InstanceData
{
  mat4 model;
};

out vec2 aNdc;
out vec2 bNdc;
out vec2 offset;
out vec4 oRGBA;

void main()
{
	vec4 aClip = vp * model * vec4(aPos_a, 1.0);
	vec4 bClip = vp * model * vec4(aPos_b, 1.0);

	aNdc = aClip.xy / aClip.w; 
	bNdc = bClip.xy / bClip.w; 

	vec2 dir = normalize(bNdc - aNdc);
	vec2 perp = vec2(-dir.y, dir.x);

	// vec2 pixelToNdc = 2.0 / viewPortSize;
	float widthPixels = 1000.;

	offset = perp * widthPixels * 0.5 * 1;//pixelToNdc;


	oRGBA = aRGBA;
}