#ifndef BUILDING_H
#define BUILDING_H

#include "geometry.h"

class Building: public Geometry {
  public:
    Building();
    virtual ~Building();
    virtual void draw(float dt);

  protected:
    Pipeline shader;
    unsigned vao;
    unsigned vbo;
};

#endif /* BUILDING_H */