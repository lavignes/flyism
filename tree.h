#ifndef TREE_H
#define TREE_H

#include "geometry.h"
#include "shader.h"
#include "maths.h"
#include "bitmap.h"

class Tree: public Geometry {
  public:
    Tree(float x, float y, float z);
    virtual ~Tree();
    virtual void draw(float dt);

  protected:
    static Pipeline* shader;
    static Bitmap* tex;
    static unsigned shader_count;
    static unsigned vao;
    static unsigned vbo;
    // const static vec3 sprite[6];
    // const static vec2 tex_st[6];
};

#endif /* TREE_H */