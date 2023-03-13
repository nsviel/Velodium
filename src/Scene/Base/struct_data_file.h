#ifndef DATAFILE_STRUCT_H
#define DATAFILE_STRUCT_H

#include <string>
#include <vector>
#include <glm/glm.hpp>


//Generique structure to parse file point clouds
struct Data_file{
  //---------------------------

  int size;
  std::string name;
  std::string path_file;
  std::string draw_type_name;

  std::vector<glm::vec3> xyz;
  std::vector<glm::vec3> Nxyz;
  std::vector<glm::vec4> rgb;
  std::vector<glm::vec2> uv;

  std::vector<float> I;
  std::vector<float> ts;
  std::vector<float> R;
  std::vector<float> A;

  std::string path_texture;

  //---------------------------
};

struct Data_file_vec{
  //---------------------------

  std::string name;
  std::string path_dir;
  std::vector<Data_file*> data_files;

  //---------------------------
};



#endif
