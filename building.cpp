#include <GL/glew.h>

#include "building.h"
#include "sim.h"

Pipeline* Building::shader = NULL;
unsigned Building::shader_count = 0;
unsigned Building::vao = 0;
unsigned Building::vbo = 0;

const vec3 Building::cube[36] = {
  vec3(-10.0f, 10.0f, 10.0f),
  vec3(-10.0f, -10.0f, 10.0f),
  vec3(10.0f, 10.0f, 10.0f),
  vec3(10.0f, 10.0f, 10.0f),
  vec3(-10.0f, -10.0f, 10.0f),
  vec3(10.0f, -10.0f, 10.0f),

  vec3(10.0f, 10.0f, 10.0f),
  vec3(10.0f, -10.0f, 10.0f),
  vec3(10.0f, 10.0f, -10.0f),
  vec3(10.0f, 10.0f, -10.0f),
  vec3(10.0f, -10.0f, 10.0f),
  vec3(10.0f, -10.0f, -10.0f),

  vec3(10.0f, 10.0f, -10.0f),
  vec3(10.0f, -10.0f, -10.0f),
  vec3(-10.0f, 10.0f, -10.0f),
  vec3(-10.0f, 10.0f, -10.0f),
  vec3(10.0f, -10.0f, -10.0f),
  vec3(-10.0f, -10.0f, -10.0f),

  vec3(-10.0f, 10.0f, -10.0f),
  vec3(-10.0f, -10.0f, -10.0f),
  vec3(-10.0f, 10.0f, 10.0f),
  vec3(-10.0f, 10.0f, 10.0f),
  vec3(-10.0f, -10.0f, -10.0f),
  vec3(-10.0f, -10.0f, 10.0f),

  vec3(-10.0f, 10.0f, -10.0f),
  vec3(-10.0f, 10.0f, 10.0f),
  vec3(10.0f, 10.0f, -10.0f),
  vec3(10.0f, 10.0f, -10.0f),
  vec3(-10.0f, 10.0f, 10.0f),
  vec3(10.0f, 10.0f, 10.0f),

  vec3(-10.0f, -10.0f, 10.0f),
  vec3(-10.0f, -10.0f, -10.0f),
  vec3(10.0f, -10.0f, 10.0f),
  vec3(10.0f, -10.0f, 10.0f),
  vec3(-10.0f, -10.0f, -10.0f),
  vec3(10.0f, -10.0f, -10.0f)
};

Building::Building(float x, float y, float z, float rot, float height):
  Geometry(x, y, z, 20.0) {
  this->rot = rot;
  this->height = height;

  // Reference counting resource
  shader = (shader)? shader : new Pipeline("bldg.vert", "bldg.frag");
  shader_count++;

  if (vao == 0 && vbo == 0) {
    glUseProgram(shader->get_id());
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

    shader->bind_attribute("coord", 0);
    glEnableVertexAttribArray(shader->get_attribute(0));
    glVertexAttribPointer(
      shader->get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

    shader->bind_uniform("mv", 0);
    shader->bind_uniform("proj", 1);
  }
}

Building::~Building() {
  shader_count--;
  if (shader_count == 0) {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    delete shader;
  }
}

void Building::draw(float dt) {
  glBindVertexArray(vao);
  glUseProgram(shader->get_id());

  mat4 mv =
    mat4::scale(1.0, height, 1.0) *
    mat4::rotate_y(rot) *
    mat4::translate(x, y, z) * Sim::get_view_matrix();

  glUniformMatrix4fv(shader->get_uniform(0), 1, false, 
    mv.as_array());
  glUniformMatrix4fv(shader->get_uniform(1), 1, false, 
    Sim::get_projection_matrix().as_array());

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  if (Sim::get_wired())
    glDrawArrays(GL_LINE_LOOP, 0, 36);
  else
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

float Building::get_rot() {
  return rot;
}

float Building::get_height() {
  return height;
}
