#version 130

out vec4 color;
in vec4 fcolor;

void main() {
  const float L1 = 2.442695;
  float z = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = exp2( -0.0002 * 0.0002 * z * z * L1 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  color = mix(vec4(0.0, 0.749019608, 1.0, 1.0), fcolor, fogFactor);
}
