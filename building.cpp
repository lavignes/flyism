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

Building::Building(float x, float y, float z, float rot, float height) {
  this->x = x;
  this->y = y;
  this->z = z;
  this->rot = rot;
  this->height = height;

  // Reference counting resource
  shader = (shader)? shader : new Pipeline("bldg.vert", "bldg.frag");
  shader_count++;

  if (vbo == 0) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
  }

  if (vao == 0) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glUseProgram(shader->get_id());

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

  mat4 mv = Sim::get_view_matrix();

  mv =
    mat4::scale(1.0, height, 1.0) *
    mat4::rotate_y(rot) *
    mat4::translate(x, y, z) * mv;

  glUniformMatrix4fv(shader->get_uniform(0), 1, false, 
    mv.as_array());
  glUniformMatrix4fv(shader->get_uniform(1), 1, false, 
    Sim::get_projection_matrix().as_array());

  glBindVertexArray(vbo);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}

float Building::get_x() {
  return x;
}

float Building::get_y() {
  return y;
}

float Building::get_z() {
  return z;
}

float Building::get_rot() {
  return rot;
}

float Building::get_height() {
  return height;
}
