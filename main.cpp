#include <stdio.h>

#include <list>

#include "sim.h"
#include "ground.h"
#include "plane.h"
#include "building.h"
#include "tree.h"
#include "maths.h"

#define DEG2RAD (M_PI * 2.0f) / 360.0f

/**
 * Ideas:
 * instrumentation (compass)
 * speed indicator
 * altimeter
 * 
 */

using namespace std;

struct SimState {
  Plane plane;
  Ground ground;
  list<Building*> buildings;
  list<Tree*> trees;
  SimState():
    plane(0.0, 100.0, 0.0),
    ground("grnd.bmp")
  {
    buildings.push_back(new Building(3200.0, 80.0, 1600.0, 16.0, 1.0));
    buildings.push_back(new Building(3000.0, 70.0, 1400.0, 45.0, 8.0));
    buildings.push_back(new Building(3160.0, 70.0, 1400.0, 45.0, 7.0));
    buildings.push_back(new Building(3264.0, 70.0, 1664.0, 0.0, 2.0));
    
    trees.push_back(new Tree(3200.0, 200.0, 1600.0));
  }
  ~SimState() {
    for (list<Building*>::iterator
      i = buildings.begin();
      i != buildings.end();
      i++)
    {
      delete *i;
    }
    for (list<Tree*>::iterator
      i = trees.begin();
      i != trees.end();
      i++)
    {
      delete *i;
    }
  }
};

void phys_callback(float dt, SimState* ss) {

  float ox = ss->plane.get_x();
  float oz = ss->plane.get_z();

  ss->plane.phys(dt);

  if (Sim::is_key_pressed('\e'))
    Sim::quit();

  if (Sim::is_key_held('a'))
    ss->plane.set_yaw(ss->plane.get_yaw() + 12.0 * dt);
  else if (Sim::is_key_held('d'))
    ss->plane.set_yaw(ss->plane.get_yaw() - 12.0 * dt);

  if (Sim::is_key_held('q'))
    ss->plane.set_roll(ss->plane.get_roll() + 12.0 * dt);
  else if (Sim::is_key_held('e'))
    ss->plane.set_roll(ss->plane.get_roll() - 12.0 * dt);

  if (Sim::is_key_held('w'))
    ss->plane.set_pitch(ss->plane.get_pitch() - 12.0 * dt);
  else if (Sim::is_key_held('s'))
    ss->plane.set_pitch(ss->plane.get_pitch() + 12.0 * dt);

  if (Sim::is_key_held('p'))
    ss->plane.set_speed(ss->plane.get_speed() + 12.0 * dt);
  else if (Sim::is_key_held('l'))
    ss->plane.set_speed(ss->plane.get_speed() - 12.0 * dt);

  // printf("(%4.2f, %4.2f) -> As: %4.2f Alt: %4.2fm Heading: %4.2f°\n",
  //   ss->plane.get_x(),
  //   ss->plane.get_z(),
  //   ss->plane.get_speed(),
  //   ss->plane.get_y(),
  //   ss->plane.get_yaw());

  for (list<Building*>::iterator
    i = ss->buildings.begin();
    i != ss->buildings.end();
    i++)
  {
    float d = sqrtf((ox-(*i)->get_x())*(ox-(*i)->get_x()) +
      (oz-(*i)->get_z())*(oz-(*i)->get_z()));
    if (d < 20.0) {
      ox += ss->plane.get_speed() * cosf(DEG2RAD * -ss->plane.get_yaw()) * dt;
      oz -= ss->plane.get_speed() * sinf(DEG2RAD * -ss->plane.get_yaw()) * dt;
      ss->plane.set_yaw(ss->plane.get_yaw() - 12.0 * dt);
      ss->plane.set_x(ox);
      ss->plane.set_z(oz);
      break;
    }
  }

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

  for (list<Building*>::iterator
    i = ss.buildings.begin();
    i != ss.buildings.end();
    i++)
  {
    Sim::add_geometry(*i);
  }

  for (list<Tree*>::iterator
    i = ss.trees.begin();
    i != ss.trees.end();
    i++)
  {
    Sim::add_geometry(*i);
  }
  
  Sim::set_phys_callback((void(*)(float, void*)) phys_callback, &ss);
  Sim::run();

  return 0;
}
