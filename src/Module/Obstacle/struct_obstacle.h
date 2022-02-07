#ifndef OBSTACLE_STRUCT_H
#define OBSTACLE_STRUCT_H

#include "../../Engine/Data/struct_glyph.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>


struct Obstac{ //Obstacle detection stuff
  //---------------------------

  std::vector<std::string> name;
  std::vector<glm::vec3> position;
  std::vector<glm::vec3> dimension;
  std::vector<float> heading;
  std::vector<Glyph*> oobb;

  //---------------------------
};

#endif
