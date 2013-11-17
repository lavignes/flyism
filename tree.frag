#version 130

in vec2 fst;

out vec4 color;

uniform sampler2D tex;

void main() {
  vec4 t = texture(tex, fst);
  if (t.a == 0.0)
    discard;
  const float L1 = 2.442695;
  float z = gl_FragCoord.z / gl_FragCoord.w;
  float fogFactor = exp2( -0.0002 * 0.0002 * z * z * L1);
  fogFactor = clamp(fogFactor, 0.0, 1.0);
  color = mix(vec4(0.0, 0.749019608, 1.0, 0.0), t, fogFactor);
}
