#ifndef BUILDING_H
#define BUILDING_H

#include "geometry.h"
#include "shader.h"
#include "maths.h"

class Building: public Geometry {
  public:
    Building(float x, float y, float z, float rot, float height);
    virtual ~Building();
    virtual void draw(float dt);
    float get_rot();
    float get_height();

  protected:
    float rot;
    float height;

    static Pipeline* shader;
    static unsigned shader_count;
    static unsigned vao;
    static unsigned vbo;
    const static vec3 cube[36];
};

#endif /* BUILDING_H */