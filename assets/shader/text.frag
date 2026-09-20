#version 460 core

in vec2 Uvs;

out vec4 FragColor;

layout(packed) uniform MaterialData
{ 
	vec4 TextColor;
};

uniform sampler2D tex;

void main()
{
	vec4 sampled = vec4(1, 1, 1, texture(tex, Uvs / textureSize(tex, 0)).r ); 
	FragColor = TextColor * sampled;
	//FragColor.a = sampled.a;
	//FragColor = vec4(0, 0, 0, 1);

}