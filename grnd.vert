#version 130

in vec3 coord;
out float h;

uniform mat4 mv;
uniform mat4 proj;
uniform float dt;

void main() {
  gl_Position = proj * mv * vec4(coord, 1.0);
  h = coord.y;
}
