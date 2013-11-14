#version 130

in vec3 coord;

uniform mat4 mv;
uniform mat4 proj;

void main() {

  gl_Position = proj * mv * vec4(coord, 1.0);
}
