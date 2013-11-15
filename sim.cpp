#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>

#include "sim.h"

#define DEG2RAD (M_PI * 2.0f) / 360.0f

Sim* Sim::self = NULL;
list<Geometry*> Sim::geoms;

vec3 Sim::cam_pos;
vec3 Sim::cam_angles;
mat4 Sim::view_matrix;
mat4 Sim::proj_matrix;
mat4 Sim::vp;

void (*Sim::callback)(void*)  = NULL;
void* Sim::cb_data = NULL;

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
  glutInitWindowSize(800, 800);
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

  view_matrix =
    mat4::translate(-cam_pos.x, -cam_pos.y, -cam_pos.z);

  // Default field rotation is 90 on y
  view_matrix = view_matrix *
                mat4::rotate_y(cam_angles.y-90) *
                mat4::rotate_x(cam_angles.x) * 
                mat4::rotate_z(cam_angles.z);
}

void Sim::reshape(int width, int height) {
  const static float near = 0.0001;
  const static float far = 3000.0;
  float aspect = float(width) / height;
  float th = tanf(45*DEG2RAD / 2);
  proj_matrix[0][0] = 1.0 / (aspect * th);
  proj_matrix[1][1] = 1.0 / th;
  proj_matrix[2][2] = (far+near)/(near-far);
  proj_matrix[3][2] = (2*far*near)/(near-far);
  proj_matrix[2][3] = -1.0;

  glViewport(0, 0, width, height);
}

void Sim::draw(int value) {
  keyboard_clear();
  look();
  if (callback)
    callback(cb_data);

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

void Sim::set_cam_x(float x) {
  cam_pos.x = x;
}

void Sim::set_cam_y(float y) {
  cam_pos.y = y;
}

void Sim::set_cam_z(float z) {
  cam_pos.z = z;
}

void Sim::set_rot_x(float x) {
  cam_angles.x = x;
}

void Sim::set_rot_y(float y) {
  cam_angles.y = y;
}

void Sim::set_rot_z(float z) {
  cam_angles.z = z;
}

mat4& Sim::get_view_matrix() {
  return view_matrix;
}

mat4& Sim::get_projection_matrix() {
  return proj_matrix;
}

void Sim::set_phys_callback(void (*cb)(void*), void* data) {
  callback = cb;
  cb_data = data;
}
