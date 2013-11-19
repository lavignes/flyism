#ifndef GEOMETRY_H
#define GEOMETRY_H

class Geometry {
  public:
    virtual void draw(float dt) = 0;
    float get_x();
    float get_y();
    float get_z();
    float get_radius();
  protected:
    Geometry(float x, float y, float z, float radius);
    float x, y, z;
    float radius;
};

#endif /* GEOMETRY_H */