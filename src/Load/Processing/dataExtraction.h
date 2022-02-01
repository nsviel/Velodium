#ifndef DATA_EXTRACTION_H
#define DATA_EXTRACTION_H

#include "../Format/struct_dataFile.h"

#include "../../common.h"
#include "../../Module/LiDAR/Velodyne/UDP/struct_UDPpacket.h"

class Scene;
class Configuration;


class dataExtraction
{
public:
  //Constructor / Destructor
  dataExtraction();
  ~dataExtraction();

public:
  //Main function
  Cloud* extractData(vector<dataFile*> data);
  Subset extractData(udpPacket* data, int ID);
  void extractData_frame(Cloud* cloud, dataFile* data);
  void extractData_oneFrame(Cloud* cloud, dataFile* data);
  void add_subsetData(Subset* subset);

private:
  //Subfunctions
  void check_data(dataFile* data);
  void check_data(udpPacket* data);
  void init_cloudParameters(Cloud* cloud, vector<dataFile*> data);
  void init_subsetParameters(Subset& subset, string path, int ID);
  void init_frameParameters(Subset& subset);
  void init_randomColor();

  void extract_Location(Subset& subset, vector<vec3>& locationOBJ);
  void extract_Color(Subset& subset, vector<vec4>& colorOBJ);
  void extract_Normal(Subset& subset, vector<vec3>& normalOBJ);
  void extract_Intensity(Subset& subset, vector<float>& intensityOBJ);
  void extract_Timestamp(Subset& subset, vector<float>& timestampOBJ);

private:
  Scene* sceneManager;
  Configuration* configManager;

  float Red, Green, Blue;
  int ID, oneFrame_VAO;
  vec4 RGB_rdm;
  bool is_intensity;
  bool is_normal;
  bool is_timestamp;
  bool is_color;
};

#endif
