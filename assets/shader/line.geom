#version 460 core
layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

uniform float line_width;

in vec2 Viewport_size[];
in vec4 oRGBA[];

out vec4 RGBA;


void main()
{
    RGBA = oRGBA[0];

    vec4 A = gl_in[0].gl_Position;
    vec4 B = gl_in[1].gl_Position;


    float da = A.z + A.w;
    float db = B.z + B.w;

    if (da < 0.0 && db < 0.0)
        return;

    if (da < 0.0)
    {
        float t = da / (da - db);
        A = mix(A, B, t);
    }

    if (db < 0.0)
    {
        float t = db / (db - da);
        B = mix(B, A, t);
    }

    vec2 a = A.xy / A.w;
    vec2 b = B.xy / B.w;

    float za = A.z / A.w;
    float zb = B.z / B.w;


    vec2 dir = b - a;
    float len = length(dir);

    if (len < 1e-6)
        return;

    vec2 perp = vec2(-dir.y, dir.x) / len;

    vec2 pixel_to_ndc = 2.0 / Viewport_size[0];

    vec2 offset = perp * (line_width * 0.5) * pixel_to_ndc;



    gl_Position = vec4(a - offset, za, 1.0);
    EmitVertex();

    gl_Position = vec4(b - offset, zb, 1.0);
    EmitVertex();

    gl_Position = vec4(a + offset, za, 1.0);
    EmitVertex();

    gl_Position = vec4(b + offset, zb, 1.0);
    EmitVertex();

    EndPrimitive();
}
