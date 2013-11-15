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
 * rotation speed should be proportional to velocity
 * 
 */

struct SimState {
  Plane plane;
  Ground ground;
  SimState(): plane(0.0, 1000.0, 0.0) {}
};

void phys_callback(SimState* ss) {

  ss->plane.phys();

  printf("%f\n", ss->plane.get_y());

  Sim::set_cam_x(ss->plane.get_x());
  Sim::set_cam_y(ss->plane.get_y());
  Sim::set_cam_z(ss->plane.get_z());
}

int main(int argc, char* argv[]) {
  Sim::init(&argc, argv, "Flyism 1.0");

  SimState ss;
  Sim::add_geometry(&ss.ground);

  Sim::set_phys_callback((void(*)(void*)) phys_callback, &ss);
  Sim::run();

  return 0;
}
