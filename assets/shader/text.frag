#version 460 core

uniform sampler2D Texture;

in vec2 Uvs;
uniform vec4 TextColor;

out vec4 FragColor;


void main()
{
	vec4 sampled = vec4(1, 1, 1, texture(Texture, Uvs / textureSize(Texture, 0)).r ); 
	FragColor = TextColor * sampled;
	//FragColor.a = sampled.a;
	//FragColor = vec4(0, 0, 0, 1);

}