#version 130

in vec3 coord;
in vec3 color;
out vec4 fcolor;

uniform mat4 mv;
uniform mat4 proj;

void main() {

  gl_Position = proj * vec4(coord, 1.0);
  fcolor = vec4(color, 1.0);
}
