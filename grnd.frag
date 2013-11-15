#version 130

out vec4 color;
in float h;

void main() {
  const float LOG2 = 1.442695;
  
  float f = exp2( -0.1 * 0.1 * -h * -h * LOG2 );
  f = clamp(f, 0.0, 1.0);
  vec4 c = mix(vec4(0.0, 0.5, 0.0, 1.0), vec4(0.2, 0.2, 0.0, 1.0), f);

  float z = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = exp2( -0.0001 * 0.0001 * z * z * LOG2 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);

  color = mix(vec4(0.0, 0.749019608, 1.0, 1.0), c, fogFactor);
}
