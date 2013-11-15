#include <math.h>

#include "plane.h"

#define DEG2RAD (M_PI * 2.0f) / 360.0f

const float Plane::air_density = 0.001;
const float Plane::prop_area = 2.0;
const float Plane::drag_area = 1.0;
const float Plane::drag_coefficient = 0.027;
const float Plane::lift_area = 16.2;
const float Plane::lift_coefficient = 3.2;
const float Plane::gravity = 9.8;
const float Plane::mass = 1114.0;

Plane::Plane(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  vx = vy = vz = 0.0;
  pitch = yaw = roll = 0.0;
  prop_speed = 500.0;
}

float Plane::get_x() {
  return x;
}

float Plane::get_y() {
  return y;
}

float Plane::get_z() {
  return z;
}

// T = .5(density)(prop_area)(prop_speed**2)
// D = .5(density)(v**2)(drag_area)(drag_coefficient)
// L = .5(density)(v**2)(lift_area)(lift_coefficient)
// G = m(gravity)
// F = T+D+L+G
// v += F/m
void Plane::phys() {
  float T = 0.5*air_density*prop_area*prop_speed*prop_speed;
  float Tx = T*cosf(DEG2RAD*pitch)*cosf(DEG2RAD*yaw);
  float Ty = T*cosf(DEG2RAD*pitch)*sinf(DEG2RAD*yaw);
  float Tz = T*sinf(DEG2RAD*pitch);
  float Dx = 0.5*air_density*drag_area*drag_coefficient*vx*vx;
  float Dy = 0.5*air_density*drag_area*drag_coefficient*vy*vy;
  float Dz = 0.5*air_density*drag_area*drag_coefficient*vz*vz;
  float Lx = 0.5*air_density*lift_area*lift_coefficient*vx*vx;
  float Ly = 0.5*air_density*lift_area*lift_coefficient*vy*vy;
  float Lz = 0.5*air_density*lift_area*lift_coefficient*vz*vz;
  float G = mass*gravity;
  float Fx = Tx-Dx+Lx;
  float Fy = Ty-Dy+Ly-G;
  float Fz = Tz-Dz+Lz;
  vx += Fx/mass*0.0166667;
  vy += Fy/mass*0.0166667;
  vz += Fz/mass*0.0166667;
  if (vz > 200.0)
    vz = 200.0;

  // x += v*t
  x += vx*0.0166667;
  y += vy*0.0166667;
  z += vz*0.0166667;

  if (y < 1.0)
    y = 1.0;
}
