#include "sim.h"
#include "ground.h"

int main(int argc, char* argv[]) {
  Sim::init(&argc, argv, "Flyism 1.0");

  Ground* g = new Ground();
  Sim::add_geometry(g);

  Sim::run();

  delete g;
  return 0;
}