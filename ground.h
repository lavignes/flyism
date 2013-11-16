#ifndef GROUND_H
#define GROUND_H

#include <string>

#include "geometry.h"
#include "maths.h"
#include "shader.h"
#include "bitmap.h"

class Ground: public Geometry {
  public:
    Ground(const std::string& texture);
    virtual ~Ground();
    virtual void draw(float dt);
    
  protected:
    Pipeline shader;
    Bitmap tex;
    unsigned n_indicies;
    unsigned vbo;
    unsigned ibo;
    unsigned vao;
};

#endif /* GROUND_H */