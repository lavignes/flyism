#include <stdio.h>

#include "sim.h"
#include "ground.h"
#include "plane.h"
/**
 * Ideas:
 * instrumentation (compass)
 * speed indicator
 * altimeter
 * 
 */

struct SimState {
  Plane plane;
  Ground ground;
  SimState(): plane(0.0, 10.0, 0.0) {}
};

void phys_callback(float dt, SimState* ss) {

  ss->plane.phys(dt);

  if (Sim::is_key_pressed('\e'))
    Sim::quit();

  if (Sim::is_key_held('a'))
    ss->plane.set_roll(ss->plane.get_roll() + 8.0 * dt);
  else if (Sim::is_key_held('d'))
    ss->plane.set_roll(ss->plane.get_roll() - 8.0 * dt);

  if (Sim::is_key_held('w'))
    ss->plane.set_pitch(ss->plane.get_pitch() - 8.0 * dt);
  else if (Sim::is_key_held('s'))
    ss->plane.set_pitch(ss->plane.get_pitch() + 8.0 * dt);

  if (Sim::is_key_held('p'))
    ss->plane.set_speed(ss->plane.get_speed() + 8.0 * dt);
  else if (Sim::is_key_held('l'))
    ss->plane.set_speed(ss->plane.get_speed() - 8.0 * dt);

  printf("(%4.2f, %4.2f) -> As: %4.2f Alt: %4.2fm Heading: %4.2f°\n",
    ss->plane.get_x(),
    ss->plane.get_z(),
    ss->plane.get_speed(),
    ss->plane.get_y(),
    ss->plane.get_yaw());

  Sim::set_cam_x(ss->plane.get_x());
  Sim::set_cam_y(ss->plane.get_y());
  Sim::set_cam_z(ss->plane.get_z());

  Sim::set_rot_x(ss->plane.get_pitch());
  Sim::set_rot_y(ss->plane.get_yaw());
  Sim::set_rot_z(ss->plane.get_roll());
}

int main(int argc, char* argv[]) {
  Sim::init(&argc, argv, "Flyism 1.0");

  SimState ss;
  Sim::add_geometry(&ss.ground);

  Sim::set_phys_callback((void(*)(float, void*)) phys_callback, &ss);
  Sim::run();

  return 0;
}
