#ifndef GROUND_H
#define GROUND_H

#include <vector>

#include "geometry.h"
#include "maths.h"
#include "shader.h"

class Ground: public Geometry {
  public:
    Ground();
    virtual ~Ground();
    virtual void draw();

  protected:
    Pipeline shader;
    unsigned n_indicies;
    unsigned vbo;
    unsigned ibo;
    unsigned vao;
};

#endif /* GROUND_H */