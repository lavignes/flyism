#version 130

in vec3 coord;
out vec3 xyz;

uniform mat4 mv;
uniform mat4 proj;

void main() {
  xyz = coord;
  gl_Position = proj * mv * vec4(coord, 1.0);
}
