#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

layout(packed) uniform ViewData { mat4 vp; };

layout(packed) uniform InstanceData { mat4 model; };

out vec2 Uvs;

void main() {
  gl_Position = vp * model * vec4(aPos, 1.0);

  Uvs = aUvs;
}