#ifndef OBSTACLE_STRUCT_H
#define OBSTACLE_STRUCT_H

#include "struct_glyph.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>


struct Obstac{ //Obstacle detection stuff
  //---------------------------

  bool is_predicted = false;
  glm::vec4 unicolor;
  std::vector<std::string> name;
  std::vector<glm::vec3> position;
  std::vector<glm::vec4> color;
  std::vector<glm::vec3> dimension;
  std::vector<float> heading;
  std::vector<Glyph> oobb;

  //---------------------------
};

#endif
