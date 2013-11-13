#include <GL/glew.h>

#include "ground.h"
#include "sim.h"

const vec3 cube[36] = {
  vec3(-0.5f, 0.5f, 0.5f),
  vec3(-0.5f, -0.5f, 0.5f),
  vec3(0.5f, 0.5f, 0.5f),
  vec3(0.5f, 0.5f, 0.5f),
  vec3(-0.5f, -0.5f, 0.5f),
  vec3(0.5f, -0.5f, 0.5f),

  vec3(0.5f, 0.5f, 0.5f),
  vec3(0.5f, -0.5f, 0.5f),
  vec3(0.5f, 0.5f, -0.5f),
  vec3(0.5f, 0.5f, -0.5f),
  vec3(0.5f, -0.5f, 0.5f),
  vec3(0.5f, -0.5f, -0.5f),

  vec3(0.5f, 0.5f, -0.5f),
  vec3(0.5f, -0.5f, -0.5f),
  vec3(-0.5f, 0.5f, -0.5f),
  vec3(-0.5f, 0.5f, -0.5f),
  vec3(0.5f, -0.5f, -0.5f),
  vec3(-0.5f, -0.5f, -0.5f),

  vec3(-0.5f, 0.5f, -0.5f),
  vec3(-0.5f, -0.5f, -0.5f),
  vec3(-0.5f, 0.5f, 0.5f),
  vec3(-0.5f, 0.5f, 0.5f),
  vec3(-0.5f, -0.5f, -0.5f),
  vec3(-0.5f, -0.5f, 0.5f),

  vec3(-0.5f, 0.5f, -0.5f),
  vec3(-0.5f, 0.5f, 0.5f),
  vec3(0.5f, 0.5f, -0.5f),
  vec3(0.5f, 0.5f, -0.5f),
  vec3(-0.5f, 0.5f, 0.5f),
  vec3(0.5f, 0.5f, 0.5f),

  vec3(-0.5f, -0.5f, 0.5f),
  vec3(-0.5f, -0.5f, -0.5f),
  vec3(0.5f, -0.5f, 0.5f),
  vec3(0.5f, -0.5f, 0.5f),
  vec3(-0.5f, -0.5f, -0.5f),
  vec3(0.5f, -0.5f, -0.5f)
};

Ground::Ground(): shader("grnd.vert", "grnd.frag") {
  for (int j = 0; j < 16; j++) {
    for (int i = 0; i < 16; i++) {
      ground_points[i][j] = vec3(i*100.0, j*100.0, 0.0);
    }
  }

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER,
    sizeof(cube), cube, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glUseProgram(shader.get_id());
  shader.bind_attribute("coord", 0);
  glEnableVertexAttribArray(shader.get_attribute(0));
  glVertexAttribPointer(
    shader.get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

  shader.bind_uniform("mv", 0);
  shader.bind_uniform("p", 1);
}

Ground::~Ground() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Ground::draw() {
  glBindVertexArray(vao);
  glBindVertexArray(vbo);

  Sim::cam_set_xpos(1.0*sin(0.0)*cos(0.0));
  Sim::cam_set_ypos(1.0*sin(0.0)*sin(0.0));
  Sim::cam_set_zpos(1.0*cos(0.0));
  Sim::cam_set_yup(1.0);

  glUniformMatrix4fv(shader.get_uniform(0), 1, true, 
    Sim::get_view_matrix().as_array());
  glUniformMatrix4fv(shader.get_uniform(1), 1, true, 
    Sim::get_projection_matrix().as_array());
  glDrawArrays(GL_TRIANGLES, 0, 36);
}