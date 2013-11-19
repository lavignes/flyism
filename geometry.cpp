#include "geometry.h"

Geometry::Geometry(float x, float y, float z, float radius) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->radius = radius;
}

float Geometry::get_x() {
  return x;
}

float Geometry::get_y() {
  return y;
}

float Geometry::get_z() {
  return z;
}

float Geometry::get_radius() {
  return radius;
}