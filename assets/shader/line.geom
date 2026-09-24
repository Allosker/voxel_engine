#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec2 aNdc[];
in vec2 bNdc[];
in vec2 offset[];

in vec4 oRGBA[];
out vec4 RGBA;

void main()
{
	RGBA = oRGBA[0];

	gl_Position = vec4(aNdc[0] - offset[0], 0., 1.);
	EmitVertex();

	gl_Position = vec4(aNdc[0] + offset[0], 0., 1.);
	EmitVertex();
	
	gl_Position = vec4(bNdc[0] - offset[0], 0., 1.);
	EmitVertex();

	gl_Position = vec4(bNdc[0] + offset[0], 0., 1.);
	EmitVertex();

	

	EndPrimitive();
}