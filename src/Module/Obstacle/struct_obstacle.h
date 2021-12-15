#ifndef OBSTACLE_STRUCT_H
#define OBSTACLE_STRUCT_H

#include "../../Engine/Data/struct_glyph.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>


struct Obstac{ //Obstacle detection stuff
  //Groung thruth
  std::vector<std::string> gt_name;
  std::vector<glm::vec3> gt_position;
  std::vector<glm::vec3> gt_dimension;
  std::vector<float> gt_heading;
  std::vector<Glyph*> gt_oobb;

  //Prediction
  std::vector<std::string> pr_name;
  std::vector<glm::vec3> pr_position;
  std::vector<glm::vec3> pr_dimension;
  std::vector<float> pr_heading;
  std::vector<Glyph*> pr_oobb;
};

#endif
