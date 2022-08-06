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
  Dimension(GLFWwindow* win, Configuration* config);
  ~Dimension();

public:
  //Main functions
  void update();
  void update_window_dim();
  void update_opengl_dim();
  void update_gui_consol();
  void update_configuration();

  //Subfunctions
  vec2 get_gl_middle();
  vec2 get_cursorPos_gl();
  vec2 get_mouse_pose();
  void set_mouse_pose(vec2 pos);

  inline GLFWwindow* get_window(){return window;}
  inline vec2* get_gui_ltp_dim(){return &gui_ltp_dim;}
  inline vec2* get_gui_ltp_pos(){return &gui_ltp_pos;}
  inline vec2* get_gui_lbp_dim(){return &gui_lbp_dim;}
  inline vec2* get_gui_lbp_pos(){return &gui_lbp_pos;}
  inline vec2* get_gui_bp_pos(){return &gui_bp_pos;}
  inline vec2* get_gui_bp_dim(){return &gui_bp_dim;}
  inline vec2* get_gui_tp_dim(){return &gui_tp_dim;}

  inline void set_gl_dim(vec2 value){this->gl_dim = value;}
  inline vec2 get_gl_dim(){return gl_dim;}
  inline vec2 get_gl_pos(){return gl_pos;}
  inline vec2 get_win_dim(){return win_dim;}
  inline void set_is_resized(bool value){this->is_resized = value;}
  inline bool get_is_resized(){return is_resized;}
  inline bool* get_with_custom_gl_dim(){return &with_custom_gl_dim;}

private:
  GLFWwindow* window;
  Configuration* configManager;

  //Rendering
  vec2 gl_pos;
  vec2 gl_dim;
  vec2 win_dim;

  //GUI
  vec2 gui_ltp_dim;
  vec2 gui_ltp_pos;
  vec2 gui_lbp_dim;
  vec2 gui_lbp_pos;
  vec2 gui_tp_dim;
  vec2 gui_bp_dim;
  vec2 gui_bp_pos;
  float gui_lp_mid;

  //Flag
  bool with_custom_gl_dim;
  bool is_visualization;
  bool is_resized;
};

#endif
