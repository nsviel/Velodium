#ifndef DIMENSION_H
#define DIMENSION_H

/**
 * \file Dimension.h
 * \brief Window dimension management
 * \author Nathan Sanchiz-Viel
 *
 * Keep information about viewport, window and UI dimensions
 *
 */
 //The ImGui origine is in the top left, and in the bottom left for OpenGL

#include "../../common.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Configuration;


class Dimension
{
public:
  Dimension(GLFWwindow* Window);
  ~Dimension();

public:
  //Main functions
  void update();
  void update_window_dim();
  void update_gl_dim();
  void update_configuration();

  //Subfunctions
  vec2 get_glMiddle();
  vec2 get_cursorPos_gl();
  vec2 get_cursorPos();
  void set_cursorPos(vec2 pos);

  inline GLFWwindow* get_window(){return window;}
  inline vec2 get_gl_dim(){return gl_dim;}
  inline vec2 get_gl_pos(){return gl_pos;}
  inline vec2 get_win_dim(){return window_dim;}
  inline float get_gui_leftPanel_width(){return gui_leftPanel_width;}
  inline float get_gui_topPanel_height(){return gui_topPanel_height;}
  inline bool get_is_resized(){return is_resized;}
  inline void set_is_resized(bool value){this->is_resized = value;}

  inline void get_gl_dim(vec2 value){this->gl_dim = value;}
  inline void set_win_dim(vec2 value){this->window_dim = value;}
  inline void set_gui_leftPanel_width(float value){this->gui_leftPanel_width = value;}
  inline void set_gui_topPanel_height(float value){this->gui_topPanel_height = value;}
  inline void set_gui_bottomPanel_height(float value){this->gui_bottomPanel_height = value;}

private:
  GLFWwindow* window;
  Configuration* configManager;

  vec2 gl_pos;
  vec2 gl_dim;
  vec2 window_dim;
  float gui_leftPanel_width;
  float gui_topPanel_height;
  float gui_bottomPanel_height;
  bool is_resized;
};

#endif
