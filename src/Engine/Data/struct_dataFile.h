#ifndef DATAFILE_STRUCT_H
#define DATAFILE_STRUCT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>


//Generique structure to parse file point clouds
struct dataFile{
  int size;
  std::string name;
  std::string path;

  std::vector<glm::vec3> location;
  std::vector<glm::vec3> normal;
  std::vector<glm::vec4> color;
  std::vector<float> intensity;
  std::vector<float> timestamp;
  std::vector<float> distance;
  std::vector<float> azimuth;
};


#endif
