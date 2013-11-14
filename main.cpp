#include "sim.h"
#include "ground.h"

/**
 * Ideas:
 * instrumentation (compass)
 * speed indicator
 * altimeter
 * 
 */

int main(int argc, char* argv[]) {
  Sim::init(&argc, argv, "Flyism 1.0");

  Ground* g = new Ground();
  Sim::add_geometry(g);

  Sim::set_cam_x(2);
  Sim::set_cam_z(2);

  Sim::run();

  delete g;
  return 0;
}