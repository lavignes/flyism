#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "sim.h"

#define deg2rad (M_PI * 2.0f) / 360.0f

Sim* Sim::self = NULL;
list<Geometry*> Sim::geoms;

vec3 Sim::cam_pos;
vec3 Sim::cam_look;
vec3 Sim::cam_up;
mat4 Sim::view_matrix;
mat4 Sim::proj_matrix;
mat4 Sim::vp;

bool Sim::key_presses[256];
bool Sim::key_releases[256];
bool Sim::key_holds[256];

void Sim::keyboard_clear() {
  for (int i = 0; i < 256; i++) {
    key_presses[i] = false;
    key_releases[i] = false;
  }
}

void Sim::keyboard_down(unsigned char key, int x, int y) {
  key_presses[key] = true;
  key_holds[key] = true;
  key_releases[key] = false;
}

void Sim::keyboard_up(unsigned char key, int x, int y) {
  key_presses[key] = false;
  key_holds[key] = false;
  key_releases[key] = true;
}

void Sim::keyboard_special_down(int key, int x, int y) {
  key_presses[key] = true;
  key_holds[key] = true;
  key_releases[key] = false;
}

void Sim::keyboard_special_up(int key, int x, int y) {
  key_presses[key] = false;
  key_holds[key] = false;
  key_releases[key] = true;
}

Sim* Sim::instance() {
  return (self)? self : self = new Sim();
}

Sim::Sim() {

}

Sim::~Sim() {

}

void Sim::init(int* argc, char* argv[], const string& title) {
  glutInit(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(800, 450);
  glutInitContextVersion(3, 1);
  glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow(title.c_str());
  glewExperimental = true;
  glewInit();

  glutKeyboardFunc(keyboard_down);
  glutKeyboardUpFunc(keyboard_up);
  glutSpecialFunc(keyboard_special_down);
  glutSpecialUpFunc(keyboard_special_up);
  glutReshapeFunc(reshape);

  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void Sim::add_geometry(Geometry* geom) {
  geoms.push_back(geom);
}

void Sim::run() {
  glutTimerFunc(16.6667, draw, 0);
  glutMainLoop();
}

void Sim::quit() {
  glutLeaveMainLoop();
}

void Sim::look() {
  vec3 n = (cam_pos-cam_look).normal();
  vec3 u3(cam_up.cross(n).normal());
  vec3 v3(n.cross(u3).normal());
  mat4 c(vec4(u3, 0.0), vec4(v3, 0.0), vec4(n, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
  view_matrix = c * mat4::translate(-cam_look.x, -cam_look.y, -cam_look.z);
}

void Sim::reshape(int width, int height) {
  
  glViewport(0, 0, width, height);

  const float near = 0.5;
  const float far = 1000.0;
  const float fov = 45.0;
  float top = tan(fov * deg2rad / 2) * near;
  float right = top * float(width)/height;
  proj_matrix[0][0] = near/right;
  proj_matrix[1][1] = near/top;
  proj_matrix[2][2] = -(far + near)/(far - near);
  proj_matrix[2][3] = -2.0*far*near/(far - near);
  proj_matrix[3][2] = -1.0;
}

void Sim::draw(int value) {
  keyboard_clear();
  look();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  list<Geometry*>::iterator g, end = geoms.end();
  for(g = geoms.begin(); g != end; g++) {
    (*g)->draw();
  }

  glutSwapBuffers();
  glutTimerFunc(16.6667, draw, 0);
}

bool Sim::is_key_pressed(int key) {
  return key_presses[key];
}

bool Sim::is_key_released(int key) {
  return key_releases[key];
}

bool Sim::is_key_held(int key) {
  return key_holds[key];
}

void Sim::cam_set_xpos(float x) {
  cam_pos.x = x;
}

void Sim::cam_set_ypos(float y) {
  cam_pos.y = y;
}

void Sim::cam_set_zpos(float z) {
  cam_pos.z = z;
}


void Sim::cam_set_xlook(float x) {
  cam_look.x = x;
}

void Sim::cam_set_ylook(float y) {
  cam_look.y = y;
}

void Sim::cam_set_zlook(float z) {
  cam_look.z = z;
}


void Sim::cam_set_xup(float x) {
  cam_up.x = x;
}

void Sim::cam_set_yup(float y) {
  cam_up.y = y;
}

void Sim::cam_set_zup(float z) {
  cam_up.z = z;
}

mat4& Sim::get_view_matrix() {
  return view_matrix;
}

mat4& Sim::get_projection_matrix() {
  return proj_matrix;
}