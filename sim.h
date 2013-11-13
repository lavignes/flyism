#ifndef SIM_H
#define SIM_H

#include <string>
#include <list>
using namespace std;

#include "geometry.h"
#include "maths.h"

class Sim {

  public:
    static Sim* instance();
    ~Sim();

    static void init(int* argc, char* argv[], const string& title);

    static void add_geometry(Geometry* geom);

    static void run();
    static void quit();

    static bool is_key_pressed(int key);
    static bool is_key_released(int key);
    static bool is_key_held(int key);

    static void cam_set_xpos(float x);
    static void cam_set_ypos(float y);
    static void cam_set_zpos(float z);

    static void cam_set_xlook(float x);
    static void cam_set_ylook(float y);
    static void cam_set_zlook(float z);

    static void cam_set_xup(float x);
    static void cam_set_yup(float y);
    static void cam_set_zup(float z);

    static mat4& get_view_matrix();
    static mat4& get_projection_matrix();

  protected:

    static vec3 cam_pos;
    static vec3 cam_look;
    static vec3 cam_up;

    static mat4 view_matrix;
    static mat4 proj_matrix;
    static mat4 vp;

    Sim();
    static void draw(int);
    static void look();
    static void reshape(int,int);
    static list<Geometry*> geoms;
    static Sim* self;
    static void keyboard_clear();
    static void keyboard_down(unsigned char, int, int);
    static void keyboard_up(unsigned char, int, int);
    static void keyboard_special_down(int, int, int);
    static void keyboard_special_up(int, int, int);
    static bool key_presses[];
    static bool key_releases[];
    static bool key_holds[];

};

#endif /* SIM_H */