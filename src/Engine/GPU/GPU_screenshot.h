#ifndef GPU_SCREENSHOT_H
#define GPU_SCREENSHOT_H

#include "../../common.h"

class Node_engine;
class Dimension;
class Configuration;
class GPU_data;


class GPU_screenshot
{
public:
  GPU_screenshot(Node_engine* node_engine);
  ~GPU_screenshot();

public:
  void render_screenshot(string path);
  void render_screenshot_stb_image(string path);
  void render_screenshot_pbo(string path);
  void render_screenshot_freeimage(string path);
  void render_screenshot_online();

  inline string* get_save_path(){return &save_path_screenshot;}
  inline bool* get_is_screenshot(){return &is_screenshot;}
  inline float get_time_screenshot(){return time_screenshot;}

private:
  Dimension* dimManager;

  GLuint pbo_screenshot;
  float time_screenshot;
  string save_path_screenshot;
  bool is_screenshot;
};

#endif
