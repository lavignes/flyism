#include <GL/glew.h>

#include "ground.h"
#include "sim.h"

const vec3 GROUND[4] = {
  vec3(-2000.0f, 0.0f, -2000.0f),
  vec3(-2000.0f, 0.0f, 2000.0f),
  vec3(2000.0f, 1.0f, -2000.0f),
  vec3(2000.0f, 1.0f, 2000.0f),
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
    sizeof(GROUND), GROUND, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glUseProgram(shader.get_id());
  shader.bind_attribute("coord", 0);
  glEnableVertexAttribArray(shader.get_attribute(0));
  glVertexAttribPointer(
    shader.get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

  shader.bind_uniform("mv", 0);
  shader.bind_uniform("proj", 1);
}

Ground::~Ground() {
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}

void Ground::draw() {
  glBindVertexArray(vao);
  glUseProgram(shader.get_id());

  glUniformMatrix4fv(shader.get_uniform(0), 1, false, 
    Sim::get_view_matrix().as_array());
  glUniformMatrix4fv(shader.get_uniform(1), 1, false, 
    Sim::get_projection_matrix().as_array());

  glBindVertexArray(vbo);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}