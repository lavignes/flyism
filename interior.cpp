#include <GL/glew.h>

#include "interior.h"
#include "sim.h"
#include "maths.h"

Pipeline* Interior::shader = NULL;
unsigned Interior::shader_count = 0;
unsigned Interior::vao = 0;
unsigned Interior::panel_vbo = 0;

const vec3 cube[36] = {
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


const vec3 colors[36] = {
  vec3(0.1, 0.1, 0.1),
  vec3(0.1, 0.1, 0.1),
  vec3(0.1, 0.1, 0.1),
  vec3(0.1, 0.1, 0.1),
  vec3(0.1, 0.1, 0.1),
  vec3(0.1, 0.1, 0.1),

  vec3(0.3, 0.3, 0.3),
  vec3(0.3, 0.3, 0.3),
  vec3(0.3, 0.3, 0.3),
  vec3(0.3, 0.3, 0.3),
  vec3(0.3, 0.3, 0.3),
  vec3(0.3, 0.3, 0.3),

  vec3(0.5, 0.5, 0.5),
  vec3(0.5, 0.5, 0.5),
  vec3(0.5, 0.5, 0.5),
  vec3(0.5, 0.5, 0.5),
  vec3(0.5, 0.5, 0.5),
  vec3(0.5, 0.5, 0.5),

  vec3(0.2, 0.2, 0.2),
  vec3(0.2, 0.2, 0.2),
  vec3(0.2, 0.2, 0.2),
  vec3(0.2, 0.2, 0.2),
  vec3(0.2, 0.2, 0.2),
  vec3(0.2, 0.2, 0.2),

  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),

  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0),
  vec3(0.0, 0.0, 0.0)
};

Interior::Interior():
  Geometry(0.0, 0.0, 0.0, 0.0)
{
  // Reference counting resource
  shader = (shader)? shader : new Pipeline("int.vert", "int.frag");
  shader_count++;

  if (vao == 0 && panel_vbo == 0) {
    glUseProgram(shader->get_id());
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &panel_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, panel_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube) + sizeof(colors), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube), cube);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube), sizeof(colors), colors);

    shader->bind_attribute("coord", 0);
    glEnableVertexAttribArray(shader->get_attribute(0));
    glVertexAttribPointer(
      shader->get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

    shader->bind_attribute("color", 1);
    glEnableVertexAttribArray(shader->get_attribute(1));
    glVertexAttribPointer(
      shader->get_attribute(1), 3, GL_FLOAT, false, 0, (void*)(sizeof(cube)));

    shader->bind_uniform("mv", 0);
    shader->bind_uniform("proj", 1);
  }
}

Interior::~Interior() {
  shader_count--;
  if (shader_count == 0) {
    glDeleteBuffers(1, &panel_vbo);
    glDeleteVertexArrays(1, &vao);
    delete shader;
  }
}

void Interior::draw(float dt) {
  glBindVertexArray(vao);
  glUseProgram(shader->get_id());

  mat4 mv;

  glUniformMatrix4fv(shader->get_uniform(0), 1, false, 
    mv.as_array());
  glUniformMatrix4fv(shader->get_uniform(1), 1, false, 
    Sim::get_ortho_matrix().as_array());

  glBindBuffer(GL_ARRAY_BUFFER, panel_vbo);
  if (Sim::get_wired())
    glDrawArrays(GL_LINE_LOOP, 0, 36);
  else
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
