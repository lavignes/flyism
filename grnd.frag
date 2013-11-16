#version 130

out vec4 color;
in float h;

void main() {
  const float LOG2 = 1.442695;

  float f = clamp(exp2( -0.08 * 0.08 * -h * -h * LOG2 ), 0.0, 1.0);
  vec4 c = mix(vec4(0.0, 0.3, 0.0, 1.0), vec4(0.0, 0.549019608, 0.8, 1.0), f);
  
  float z = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = exp2( -0.0002 * 0.0002 * z * z * LOG2 );
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  color = mix(vec4(0.0, 0.749019608, 1.0, 1.0), c, fogFactor);
}
