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
  Extractor();
  ~Extractor();

public:
  //Main function
  Collection* extract_data(vector<Data_file*> data);
  Object_* extract_data_object(Data_file* data);
  Subset* extract_data(Data_file& data);
  void extract_data(Collection* cloud, Data_file* data);

private:
  //Init function
  void check_data(Data_file* data);
  void init_cloud_parameter(Collection* cloud, vector<Data_file*> data);
  void init_subset_parameter(Subset* subset, Data_file* data, int ID);
  void init_object_parameter(Object_* object, Data_file* data, int ID);
  void init_random_color();

  //Param function
  void define_visibility(Subset* subset, int i);
  void define_buffer_init(Collection* cloud, Subset* subset);
  void compute_texture(Subset* subset, Data_file* data);

private:
  Object* objectManager;
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
