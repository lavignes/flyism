#ifndef PLANE_H
#define PLANE_H

class Plane {
  public:
    Plane(float x, float y, float z);
    void phys();

    float get_x();
    float get_y();
    float get_z();
    
  private:
    float x, y, z;
    float vx, vy, vz;
    float prop_speed;
    float pitch;
    float yaw;
    float roll;

    const static float air_density;
    const static float prop_area;
    const static float drag_area;
    const static float drag_coefficient;
    const static float lift_area;
    const static float lift_coefficient;
    const static float gravity;
    const static float mass;
};

#endif /* PLANE_H */
