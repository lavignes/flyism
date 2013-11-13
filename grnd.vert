#version 130

in vec3 coord;

uniform mat4 mv;
uniform mat4 p;

void main() {
  gl_Position = vec4(coord, 1.0);
}
