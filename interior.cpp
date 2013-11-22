#include <GL/glew.h>
#include <stdio.h>

#include "interior.h"
#include "sim.h"
#include "maths.h"

Pipeline* Interior::shader = NULL;
unsigned Interior::shader_count = 0;
unsigned Interior::vao = 0;
unsigned Interior::gimble_vbo = 0;

Interior::Interior():
  Geometry(0.0, 0.0, 0.0, 0.0)
{
  // Reference counting resource
  shader = (shader)? shader : new Pipeline("int.vert", "int.frag");
  shader_count++;

  if (vao == 0 && gimble_vbo == 0) {
    float resolution = 16.0;
    vec3 verts[578]; // resolution+1 * resolution+1 * 2
    int n = 0;
    for(int i = 0; i <= resolution; i++)
    {
      float lat0 = M_PI * (-0.5 + (float) (i - 1) / resolution);
      float z0  = 100.0 * sinf(lat0);
      float zr0 = 100.0 * cosf(lat0);

      float lat1 = M_PI * (-0.5 + (float) i / resolution);
      float z1 = 100.0 * sinf(lat1);
      float zr1 = 100.0 * cosf(lat1);

      for(int j = 0; j <= resolution; j++)
      {
        float lng = 2 * M_PI * (float) (j - 1) / resolution;
        float x = cosf(lng);
        float y = sinf(lng);

        verts[n] = vec3(x * zr0, y * zr0, z0);
        n++;

        verts[n] = vec3(x * zr1, y * zr1, z1);
        n++;
      }
    }

    glUseProgram(shader->get_id());
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &gimble_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gimble_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    shader->bind_attribute("coord", 0);
    glEnableVertexAttribArray(shader->get_attribute(0));
    glVertexAttribPointer(
      shader->get_attribute(0), 3, GL_FLOAT, false, 0, (void*)(0));

    shader->bind_uniform("mv", 0);
    shader->bind_uniform("proj", 1);
    shader->bind_uniform("tex", 2);
  }
}

Interior::~Interior() {
  shader_count--;
  if (shader_count == 0) {
    glDeleteBuffers(1, &gimble_vbo);
    glDeleteVertexArrays(1, &vao);
    delete shader;
  }
}

void Interior::draw(float dt) {
  glBindVertexArray(vao);
  glUseProgram(shader->get_id());

  mat4 mv = mat4::rotate_y(-Sim::get_rot_y()) *
            mat4::rotate_x(Sim::get_rot_x()) *
            mat4::rotate_z(Sim::get_rot_z()) *
            mat4::translate(0.0, 64.0, 0.0);

  glUniformMatrix4fv(shader->get_uniform(0), 1, false, 
    mv.as_array());
  glUniformMatrix4fv(shader->get_uniform(1), 1, false, 
    Sim::get_ortho_matrix().as_array());

  glBindBuffer(GL_ARRAY_BUFFER, gimble_vbo);
  if (Sim::get_wired())
    glDrawArrays(GL_LINE_STRIP, 0, 578);
  else
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 578);
}
