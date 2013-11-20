#include <stdio.h>
#include <stdlib.h>

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
  list<Geometry*> solids;
  SimState():
    plane(3006.0, 58.30, 1180.0, -70.20),
    ground("grnd.bmp")
  {
    solids.push_back(new Building(3200.0, 70.0, 1600.0, 16.0, 1.0));
    solids.push_back(new Building(3232.0, 70.0, 1664.0, 16.0, 1.0));
    solids.push_back(new Building(3264.0, 70.0, 1632.0, 16.0, 1.0));

    solids.push_back(new Building(3000.0, 70.0, 1400.0, 45.0, 8.0));
    solids.push_back(new Building(3032.0, 70.0, 1432.0, 16.0, 2.0));
    solids.push_back(new Building(3300.0, 70.0, 1400.0, 45.0, 8.0));
    solids.push_back(new Building(3160.0, 70.0, 1400.0, 45.0, 7.0));
    solids.push_back(new Building(3264.0, 70.0, 1664.0, 0.0, 2.0));
    solids.push_back(new Tree(3000.0, 65.0, 1200.0));
    solids.push_back(new Tree(3010.0, 65.0, 1220.0));
    solids.push_back(new Tree(3020.0, 65.0, 1200.0));
    solids.push_back(new Tree(3296.0, 75.0, 1596.0));
  }
  ~SimState() {
    for (list<Geometry*>::iterator
      solid = solids.begin();
      solid != solids.end();
      solid++)
    {
      delete *solid;
    }
  }
};

void phys_callback(float dt, SimState* ss) {

  float ox = ss->plane.get_x();
  float oy = ss->plane.get_y();
  float oz = ss->plane.get_z();

  ss->plane.phys(dt);

  if (Sim::is_key_pressed('\e'))
    Sim::quit();

  if (Sim::is_key_pressed('='))
    Sim::set_wired(true);
  else if (Sim::is_key_pressed('-'))
    Sim::set_wired(false);

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

  for (list<Geometry*>::iterator
    solid = ss->solids.begin();
    solid != ss->solids.end();
    solid++)
  {
    float d = sqrtf((ox-(*solid)->get_x())*(ox-(*solid)->get_x()) +
      (oz-(*solid)->get_z())*(oz-(*solid)->get_z()));
    if (d < (*solid)->get_radius()) {
      ox += ss->plane.get_speed() * cosf(DEG2RAD * -ss->plane.get_yaw()) * dt;
      oz -= ss->plane.get_speed() * sinf(DEG2RAD * -ss->plane.get_yaw()) * dt;
      ss->plane.set_yaw(ss->plane.get_yaw() - 36.0 * dt);
      ss->plane.set_x(ox);
      ss->plane.set_z(oz);
      break;
    }
  }

  float h = ss->ground.get_height_at(ox, oz) + 8.0;
  if (oy < h)
    ss->plane.set_y(h);

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

  for (list<Geometry*>::iterator
    solid = ss.solids.begin();
    solid != ss.solids.end();
    solid++)
  {
    Sim::add_geometry(*solid);
  }

  Sim::set_phys_callback((void(*)(float, void*)) phys_callback, &ss);
  Sim::run();

  return 0;
}
