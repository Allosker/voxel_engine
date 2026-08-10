#version 460 core
layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

/*
* Directly build the faces inside of the shader
*  Note: the facing node should be given, not 
*/

uniform mat4 model;
uniform mat4 vp;

in vec2 Uvs[];
out vec2 gsUvs;


void main()
{

	// Front Face
	gsUvs = vec2(0, 0);
	gl_Position = vp * model * (gl_in[0].gl_Position + vec4(-0.5, -0.5, 0, 0));
	EmitVertex();

	gsUvs = vec2(1, 0);
	gl_Position = vp * model * (gl_in[0].gl_Position + vec4(0.5, -0.5, 0, 0));
	EmitVertex();

	gsUvs = vec2(0, 1);
	gl_Position = vp * model * (gl_in[0].gl_Position + vec4(-0.5, 0.5, 0, 0));
	EmitVertex();

	gsUvs = vec2(1, 1);
	gl_Position = vp * model * (gl_in[0].gl_Position + vec4(0.5, 0.5, 0, 0));
	EmitVertex();


	EndPrimitive();
}