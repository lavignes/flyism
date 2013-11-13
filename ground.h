#ifndef GROUND_H
#define GROUND_H

#include "geometry.h"
#include "maths.h"
#include "shader.h"

class Ground: public Geometry {
  public:
    Ground();
    virtual ~Ground();
    virtual void draw();

  protected:
    vec3 ground_points[16][16];
    Pipeline shader;
    unsigned vbo;
    unsigned vao;
};

#endif /* GROUND_H */