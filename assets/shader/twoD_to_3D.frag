#version 460 core

in vec2 Uvs;
in vec3 fragPos;

uniform sampler2D tex;


out vec4 FragColor;

void main()
{
	// Calculate the normal by getting two tangent vectors to the surface and calculating their cross product
	vec3 norm = normalize(cross(dFdx(fragPos), dFdy(fragPos)));

	//vec3 lightPos = vec3(2000, 1000, 0);
	vec3 lightPos = vec3(3, -0.8, 0);
	vec3 lightColor = vec3(1, 1, 1);
	vec3 ambient = vec3(0.6, 0.6, 0.6);

	vec3 lightDir = normalize(lightPos - fragPos);


	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse);
	FragColor = vec4(result, 1.0) * texture(tex, Uvs);
}
