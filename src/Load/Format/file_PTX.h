#ifndef FILE_PTX_H
#define FILE_PTX_H

#include "../../Scene/Base/struct_data_file.h"
#include "../../Module/Registration/struct_misc.h"
#include "../../common.h"

#include <iomanip>
#include <fstream>

struct PTXCloud{
  //---------------------------

  //header
  glm::vec3 rootTrans;
  glm::mat3 rootRotat;
  glm::mat4 transfMat;

  //data
  std::vector<glm::vec3> location;
  std::vector<float> intensity;
  std::vector<glm::vec4> color;

  //---------------------------
};


class file_PTX
{
public:
  //Constructor / Destructor
  file_PTX();
  ~file_PTX();

public:
  //Main functions
  Data_file* Loader(string pathFile);
  bool Exporter(string pathFile);

  //Subfunctions
  void Loader_header(PTXCloud* cloud);
  void Loader_data(PTXCloud* cloud);
  void Loader_assembling();
  void Loader_cloudTransformation();
  void Loader_scannerAtOrigin();

  string get_format_from_path(string path);

  inline void set_scannerAtOrigin(bool value){this->option_scannerAtOrigin = value;}
  inline void set_applyCloudTransfo(bool value){this->option_applyCloudTransfo = value;}
  inline void set_separateCloud(bool value){this->option_separateCloud = value;}
  inline void set_IdataFormat(int value){this->IdataFormat = value;}
  inline void set_retrievingIntensity(bool value){this->retrieve_I = value;}
  inline void set_retrievingColor(bool value){this->retrieve_RGB = value;}
  inline void set_retrievingNormal(bool value){this->retrieve_N = value;}
  inline void set_notUseZValue(bool value){this->option_notUseZValue = value;}

private:
  //Datatypes
  Data_file* data_out;

  list<PTXCloud*>* list_ptxCloud;
  float x, y, z, I, r, g ,b;
  int PC_line;

  int IdataFormat;
  bool option_separateCloud;
  bool option_scannerAtOrigin;
  bool option_applyCloudTransfo;
  bool option_notUseZValue;
  bool retrieve_I, retrieve_RGB, retrieve_N;
};

#endif
