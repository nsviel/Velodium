#ifndef FILE_PTX_H
#define FILE_PTX_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../Engine/Data/struct_generic.h"
#include "../../common.h"

#include <iomanip>
#include <fstream>


class file_PTX
{
public:
  //Constructor / Destructor
  file_PTX();
  ~file_PTX();

public:
  //Main functions
  dataFile* Loader(string pathFile);
  bool Exporter(string pathFile);

  //Subfunctions
  void Loader_header(PTXCloud* cloud);
  void Loader_data(PTXCloud* cloud);
  void Loader_assembling();
  void Loader_cloudTransformation();
  void Loader_scannerAtOrigin();

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
  dataFile* data_out;

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
