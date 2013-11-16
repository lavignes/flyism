#ifndef INTERIOR_H
#define INTERIOR_H

#include "geometry.h"
#include "shader.h"

class Interior: public Geometry {
  public:
    Interior();
    virtual ~Interior();
    virtual void draw(float dt);

  protected:
    Pipeline shader;
    unsigned vao;
    unsigned panel_vbo;
    unsigned dial_vbo;
    unsigned compass_vbo;
};

#endif /* INTERIOR_H */