#include <math.h>

#include "plane.h"

#define DEG2RAD (M_PI * 2.0f) / 360.0f

Plane::Plane(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;
  pitch = yaw = roll = 0.0;
  v = 0.0;
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

float Plane::get_roll() {
  return roll;
}

void Plane::set_roll(float roll) {
  this->roll = roll;
}

float Plane::get_pitch() {
  return pitch;
}

void Plane::set_pitch(float pitch) {
  this->pitch = pitch;
}

float Plane::get_yaw() {
  return yaw;
}

float Plane::get_speed() {
  return v;
}

void Plane::set_speed(float speed) {
  v = speed;
}

void Plane::phys(float dt) {
  int n =  -2 * ((roll > 90.0) || (roll < -90.0)) + 1;

  float yaw_d = asinf(sinf(DEG2RAD * roll) * sinf(DEG2RAD * pitch)) * 30.0;
  if (yaw_d > 8.0)
    yaw_d = 8.0;
  else if (yaw_d < -8.0)
    yaw_d = -8.0;
  yaw += yaw_d*dt;

  x += v*cosf(DEG2RAD * yaw) * dt;
  z -= v*sinf(DEG2RAD * yaw) * dt;
  y += n * (v * sinf(DEG2RAD * pitch) - v * fabs(sinf(DEG2RAD * roll))) * dt;

  if (y < 1.0)
    y = 1.0;
}
