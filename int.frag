#version 130

in vec3 xyz;
out vec4 color;

void main() {
  if (xyz.y > 0)
    color = vec4(1.0, 1.0, 1.0, 0.3);
  else
    color = vec4(0.0, 0.0, 0.0, 0.3);

  if (xyz.x > 65.0 && xyz.z > 65.0 && xyz.y > 0.0)
    color = vec4(1.0, 1.0, 0.0, 1.0);

  else if (xyz.x > 65.0 && xyz.z < -65.0 && xyz.y > 0.0)
    color = vec4(0.0, 0.0, 0.0, 1.0);

  else if (xyz.x < -65.0 && xyz.z < -65.0 && xyz.y > 0.0)
    color = vec4(0.0, 0.0, 1.0, 1.0);

  else if (xyz.x < -65.0 && xyz.z > 65.0 && xyz.y > 0.0)
    color = vec4(1.0, 0.0, 0.0, 1.0);
}
