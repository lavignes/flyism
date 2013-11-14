#include <stdio.h>

#include "sim.h"
#include "ground.h"
/**
 * Ideas:
 * instrumentation (compass)
 * speed indicator
 * altimeter
 *
 * rotation speed should be proportional to velocity
 * 
 */

struct SimState {
  float x, y, z;
  float tx, ty, tz;
  float v;
  Ground* ground;
};

void phys_callback(SimState* ss) {

  if (Sim::is_key_held('a')) {
    ss->ty += 0.5;
    ss->tz += 0.1;
    ss->v -= 0.0001;
  } else if (Sim::is_key_held('d')) {
    ss->ty -= 0.5;
    ss->tz -= 0.1;
    ss->v -= 0.0001;
  }

  if (Sim::is_key_held('p')) {
    ss->v += 0.001;
  } else if (Sim::is_key_held('l')) {
    ss->v -= 0.001;
  }

  if (Sim::is_key_held('w')) {
    ss->tx -= 1.0;
  } else if (Sim::is_key_held('s')) {
    ss->tx += 1.0;
  }

  if (Sim::is_key_held('q')) {
    ss->tz += 1.0;
  } else if (Sim::is_key_held('e')) {
    ss->tz -= 1.0;
  }

  float a = sinf(DEG2RAD * ss->tx);
  if (a < 0.0) {
    ss->v += 0.0001;
  } else if (a > 0.0) {
    ss->v -= 0.0001;
  }

  printf("(%4.2f, %4.2f) %4.2f %4.2f\n", ss->x, ss->z, ss->y, ss->tz);

  if (ss->tz > 0.0)
    ss->tz -= 0.05;
  else if (ss->tz < 0.0)
    ss->tz += 0.05;

  ss->y -= 0.0001; // Gravity

  if (ss->v < 0.0)
    ss->v = 0.0;

  ss->x += ss->v * cosf(DEG2RAD * ss->ty);
  ss->y += ss->v * a;
  ss->z -= ss->v * sinf(DEG2RAD * ss->ty);

  Sim::set_rot_x(ss->tx);
  Sim::set_rot_y(ss->ty);
  Sim::set_rot_z(ss->tz);
  Sim::set_cam_x(ss->x);
  Sim::set_cam_y(ss->y);
  Sim::set_cam_z(ss->z);
}

int main(int argc, char* argv[]) {
  Sim::init(&argc, argv, "Flyism 1.0");

  SimState ss = {
    x: 0.0, y: 10.0, z: 2.0,
    tx: 0.0, ty: 0.0, tz: 0.0,
    v: 0.0,
    ground: new Ground(),
  };

  Sim::add_geometry(ss.ground);

  Sim::set_phys_callback((void(*)(void*)) phys_callback, &ss);
  Sim::run();

  delete ss.ground;
  return 0;
}