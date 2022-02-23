#ifndef FILE_PTS_H
#define FILE_PTS_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"

#include <iomanip>
#include <fstream>


class file_PTS
{
public:
  //Constructor / Destructor
  file_PTS();
  ~file_PTS();

public:
  //Main functions
  dataFile* Loader(string pathFile);
  dataFile* Loader(string pathFile, int lmin, int lmax);

  bool Exporter(string pathFile, Cloud* cloud);
  bool Exporter(string pathFile, Subset* subset);

  inline void set_IdataFormat(int value){this->IdataFormat = value;}
  inline void set_retrievingIntensity(bool value){this->retrieve_I = value;}
  inline void set_retrievingColor(bool value){this->retrieve_RGB = value;}
  inline void set_retrievingNormal(bool value){this->retrieve_N = value;}

private:
  //Subfunctions
  void Loader_init();
  void Loader_nbColumns();
  void Loader_configuration();
  void Loader_data(int FILE_config);

  //Loader sub-functions
  bool check_header(string pathFile);
  int check_configuration(string pathFile);
  int check_size(string pathFile, bool FILE_hasHeader);

private:
  //Variables
  int config;
  int nbptMax;
  int FILE_size, FILE_config;
  bool endHeader, FILE_hasHeader;
  bool endParameters;
  bool hasColor;
  bool hasIntensity;
  bool hasNormal;
  vector<float> line_columns;
  vector<string> dataFormat;
  string line;

  //Parameters
  int IdataFormat, export_IdataFormat;
  bool retrieve_I, retrieve_RGB, retrieve_N;

  //Datatypes
  dataFile* data_out;
};

#endif
