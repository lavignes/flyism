#version 130

out vec4 color;
in float h;

void main() {
  color = vec4(0.0, 1.0-h, 0.0, 1.0);
}
