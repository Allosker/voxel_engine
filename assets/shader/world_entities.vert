#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUvs;

uniform mat4 vp;

uniform mat4 model;

out vec2 Uvs;

void main() {
  gl_Position = vec4(aPos, 1.0);

  Uvs = aUvs;
}