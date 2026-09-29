#version 460 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 oRGBA[];

uniform vec2 view_port_size;

out vec4 RGBA;


void main()
{
	RGBA = oRGBA[0];

	float widthPixels = 10.;
	vec2 pixel_to_ndc = vec2(2. / view_port_size.x, 2. / view_port_size.y);


	vec2 a = gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w;
	vec2 b = gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w;

	vec2 dir = normalize(b - a);
	vec2 perp = vec2(-dir.y, dir.x);

	vec2 offset = perp * widthPixels * 0.5 * pixel_to_ndc;

	float za = gl_in[0].gl_Position.z / gl_in[0].gl_Position.w;
	float zb = gl_in[1].gl_Position.z / gl_in[1].gl_Position.w;
	

	gl_Position = vec4(a + offset, za, 1.);
	EmitVertex();

	gl_Position = vec4(a - offset, za, 1.);
	EmitVertex();
	
	gl_Position = vec4(b + offset, zb, 1.);
	EmitVertex();

	gl_Position = vec4(b - offset, zb, 1.);
	EmitVertex();

	

	EndPrimitive();
}