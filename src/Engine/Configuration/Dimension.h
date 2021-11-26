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

class Dimension
{
public:
  Dimension(GLFWwindow* Window);
  ~Dimension();

public:
  //Main functions
  void update_window_dim();
  void update_viewport_dim();
  void update_configuration();

  //Subfunctions
  vec2 get_glMiddle();
  vec2 get_cursorPos_gl();
  vec2 get_cursorPos();
  void set_cursorPos(vec2 pos);

  inline GLFWwindow* get_window(){return window;}
  inline vec2 get_glDim(){return viewport_dim;}
  inline vec2 get_glPos(){return viewport_pos;}
  inline vec2 get_winDim(){return window_dim;}
  inline vec2 get_guiDim_lP(){return gui_leftPanel_dim;}
  inline vec2 get_guiDim_tP(){return gui_topPanel_dim;}
  inline void set_glDim(vec2 value){this->viewport_dim = value;}
  inline void set_guiDim_lP(vec2 value){this->gui_leftPanel_dim = value; update_viewport_dim();}
  inline void set_guiDim_tP(vec2 value){this->gui_topPanel_dim = value; update_viewport_dim();}
  inline void set_winDim(vec2 value){this->window_dim = value; update_viewport_dim();}
  inline void set_guiDim_bP(vec2 value){this->gui_bottomPanel_dim = value; update_viewport_dim();}

private:
  GLFWwindow* window;

  vec2 viewport_dim;
  vec2 viewport_pos;
  vec2 window_dim;
  vec2 gui_leftPanel_dim;
  vec2 gui_topPanel_dim;
  vec2 gui_bottomPanel_dim;
};

#endif
