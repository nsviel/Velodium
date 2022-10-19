#ifndef DATA_EXTRACTION_H
#define DATA_EXTRACTION_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../Engine/Data/struct_UDPpacket.h"

#include "../../common.h"

class Node_load;
class Scene;
class Object;
class Configuration;


class Extractor
{
public:
  //Constructor / Destructor
  Extractor(Node_load* node_load);
  ~Extractor();

public:
  //Main function
  Cloud* extract_data(vector<dataFile*> data);
  Subset* extract_data(udpPacket& data);
  void extract_data_frame(Cloud* cloud, dataFile* data);
  void extract_data_oneFrame(Cloud* cloud, dataFile* data);

private:
  //Subfunctions
  void check_data(dataFile* data);
  void check_data(udpPacket& data);
  void init_cloud_parameter(Cloud* cloud, vector<dataFile*> data);
  void init_subset_parameter(Subset* subset, string path, int ID);
  void init_frame_parameter(Subset* subset);
  void init_random_color();

  void extract_location(Subset* subset, vector<vec3>& locationOBJ);
  void extract_color(Subset* subset, vector<vec4>& colorOBJ);
  void extract_normal(Subset* subset, vector<vec3>& normalOBJ);
  void extract_intensity(Subset* subset, vector<float>& intensityOBJ);
  void extract_timestamp(Subset* subset, vector<float>& timestampOBJ);

private:
  Scene* sceneManager;
  Object* objectManager;
  Configuration* configManager;

  int ID;
  vec4 color_rdm;
  bool is_intensity;
  bool is_normal;
  bool is_timestamp;
  bool is_color;
};

#endif
