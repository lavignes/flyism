#version 130

in vec3 coord;
out vec4 fcolor;

uniform mat4 mv;
uniform mat4 proj;

void main() {
  const float L2 = 0.333333;
  float f = clamp(exp2( -0.08 * 0.08 * -coord.y * -coord.y * L2), 0.0, 1.0);
  fcolor = mix(vec4(0.1, 0.2, -0.4, 1.0), vec4(0.0, 0.549019608, 0.8, 1.0), f);

  gl_Position = proj * mv * vec4(coord, 1.0);
}
