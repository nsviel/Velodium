#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../../common.h"

class Glyphs;


class Obstacle
{
public:
  //Constructor / Destructor
  Obstacle();
  ~Obstacle();

public:
  void run();
  void parse_json(string path);
  void build_aabb();

private:
  Glyphs* glyphManager;

  vector<string> detect_name;
  vector<vec3> detect_posi;
  vector<vec3> detect_dime;
  vector<float> detect_head;
};

#endif
