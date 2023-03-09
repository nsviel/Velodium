#ifndef DATA_EXTRACTION_H
#define DATA_EXTRACTION_H

#include "../../Scene/Base/struct_data_file.h"
#include "../../Scene/Base/struct_data_file.h"

#include "../../common.h"

class Node_load;
class Scene;
class Object;
class Configuration;
class Texture;
class GPU_data;


class Extractor
{
public:
  //Constructor / Destructor
  Extractor(Node_load* node_load);
  ~Extractor();

public:
  //Main function
  Cloud* extract_data(vector<Data_file*> data);
  Subset* extract_data(Data_file& data);
  void extract_data(Cloud* cloud, Data_file* data);

private:
  //Init function
  void check_data(Data_file* data);
  void init_cloud_parameter(Cloud* cloud, vector<Data_file*> data);
  void init_subset_parameter(Subset* subset, Data_file* data, int ID);
  void init_random_color();

  //Param function
  void define_visibility(Subset* subset, int i);
  void define_buffer_init(Cloud* cloud, Subset* subset);
  void compute_texture(Subset* subset, Data_file* data);

private:
  Scene* sceneManager;
  Object* objectManager;
  Configuration* configManager;
  Texture* texManager;
  GPU_data* gpuManager;

  int ID;
  int ID_cloud;
  vec4 color_rdm;
  bool is_intensity;
  bool is_normal;
  bool is_timestamp;
  bool is_color;
  bool is_texture;
};

#endif
