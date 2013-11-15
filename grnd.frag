#version 130

out vec4 color;
in float h;

void main() {
  color = vec4(0.0, 1.0-h, 0.0, 1.0);

  const float LOG2 = 1.442695;
  float z = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = exp2( -0.0005 * 0.0005 * z * z * LOG2 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  color = mix(vec4(0.8, 0.8, 0.8, 1.0), vec4(0.0, 1.0, 0.0, 1.0), fogFactor);
}
