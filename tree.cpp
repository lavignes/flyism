#include <stdio.h>

#include <GL/glew.h>

#include "tree.h"
#include "sim.h"

Pipeline* Tree::shader = NULL;
Bitmap* Tree::tex = NULL;
unsigned Tree::shader_count = 0;
unsigned Tree::vao = 0;
unsigned Tree::vbo = 0;

const static vec3 sprite[6] = {
  vec3(-500.0f, 500.0f, 0.0f),
  vec3(-500.0f, -500.0f, 0.0f),
  vec3(500.0f, 500.0f, 0.0f),
  vec3(500.0f, 500.0f, 0.0f),
  vec3(-500.0f, -500.0f, 0.0f),
  vec3(500.0f, -500.0f, 0.0f),
};

const static vec2 tex_st[6] = {
  vec2(0.0f, 0.0f),
  vec2(0.0f, 1.0f),
  vec2(1.0f, 0.0f),
  vec2(1.0f, 0.0f),
  vec2(0.0f, 1.0f),
  vec2(1.0f, 1.0f),
};

Tree::Tree(float x, float y, float z) {
  this->x = x;
  this->y = y;
  this->z = z;

  // Reference counting resource
  shader = (shader)? shader : new Pipeline("tree.vert", "tree.frag");
  shader_count++;
  tex = (tex)? tex : new Bitmap("grnd.bmp");

  if (vao == 0 && vbo == 0) {
    glUseProgram(shader->get_id());
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData( GL_ARRAY_BUFFER, sizeof(sprite) + sizeof(tex_st), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sprite), sprite);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(sprite), sizeof(tex_st), tex_st);

    shader->bind_attribute("coord", 0);
    glEnableVertexAttribArray(shader->get_attribute(0));
    glVertexAttribPointer(
      shader->get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));
    
    shader->bind_attribute("st", 1);
    glEnableVertexAttribArray(shader->get_attribute(1));
    glVertexAttribPointer(
      shader->get_attribute(1), 2, GL_FLOAT, false, 0, (void*)(sizeof(sprite)));

    shader->bind_uniform("mv", 0);
    shader->bind_uniform("proj", 1);
    shader->bind_uniform("tex", 2);
  }
}

Tree::~Tree() {
  shader_count--;
  if (shader_count == 0) {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete shader;
    delete tex;
  }
}

void Tree::draw(float dt) {
  glUseProgram(shader->get_id());
  glBindVertexArray(vao);

  mat4 mv = Sim::get_view_matrix();

  mv = mat4::translate(x, y, z) * mv;

  glUniformMatrix4fv(shader->get_uniform(0), 1, false, 
    mv.as_array());
  glUniformMatrix4fv(shader->get_uniform(1), 1, false, 
    Sim::get_projection_matrix().as_array());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex->get_id());
  glUniform1i(shader->get_uniform(2), 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}