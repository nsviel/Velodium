#ifndef DATA_EXTRACTION_H
#define DATA_EXTRACTION_H

#include "../../Scene/Struct/struct_data_file.h"
#include "../../Scene/Struct/struct_data_cap.h"

#include "../../common.h"

#include "../../Scene/Base/Subset_base.h"
#include "../../Scene/Base/Mesh_base.h"

class Node_load;
class Scene;
class Object;
class Configuration;
class Texture;
class GPU_transfert;


class Extractor
{
public:
  //Constructor / Destructor
  Extractor(Node_load* node_load);
  ~Extractor();

public:
  //Main function
  Cloud* extract_data(vector<Data_file*> data);
  Subset* extract_data(Data_cap& data);
  void extract_data_frame(Cloud* cloud, Data_file* data);
  void extract_data_oneFrame(Cloud* cloud, Data_file* data);

  //Base
  Cloud_base* extract_data_(vector<Data_file*> data);
  void init_cloud_parameter(Cloud_base* cloud, vector<Data_file*> data);

private:
  //Subfunctions
  void check_data(Data_file* data);
  void check_data(Data_cap& data);
  void init_cloud_parameter(Cloud* cloud, vector<Data_file*> data);
  void init_subset_parameter(Subset* subset, string path, int ID);
  void init_random_color();
  void define_visibility(Subset* subset, int i);
  void define_buffer_init(Cloud* cloud, Subset* subset);

  void extract_location(Subset* subset, vector<vec3>& locationOBJ);
  void extract_color(Subset* subset, vector<vec4>& colorOBJ);
  void extract_normal(Subset* subset, vector<vec3>& normalOBJ);
  void extract_intensity(Subset* subset, vector<float>& intensityOBJ);
  void extract_timestamp(Subset* subset, vector<float>& timestampOBJ);
  void extract_texture(Subset* subset, vector<vec2>& vec_tex);

  void compute_texture(Subset* subset, Data_file* data);




private:
  Scene* sceneManager;
  Object* objectManager;
  Configuration* configManager;
  Texture* texManager;
  GPU_transfert* gpuManager;

  int ID;
  vec4 color_rdm;
  bool is_intensity;
  bool is_normal;
  bool is_timestamp;
  bool is_color;
  bool is_texture;
};

#endif
