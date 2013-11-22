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
    static Pipeline* shader;
    static unsigned shader_count;
    static unsigned vao;
    static unsigned panel_vbo;
};

#endif /* INTERIOR_H */
