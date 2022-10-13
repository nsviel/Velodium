#ifndef FILE_PLY_H
#define FILE_PLY_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"

#include <iomanip>
#include <fstream>


class file_PLY
{
public:
  //Constructor / Destructor
  file_PLY();
  ~file_PLY();

public:
  //Main functions
  dataFile* Loader(string path_file);
  bool Exporter_cloud(string path_file, string format, Cloud* cloud);
  bool Exporter_subset(string path_dir, string format, Subset* subset);
  bool Exporter_subset(string path_dir, string format, Subset* subset, string fileName);

private:
  //Loader subfunctions
  void Loader_header(std::ifstream& file);
  void Loader_data_ascii(std::ifstream& file);
  void Loader_data_binary(std::ifstream& file);
  void reorder_by_timestamp();

  //Exporter subfunctions
  void Exporter_header(std::ofstream& file, string format, Subset* subset);
  void Exporter_data_ascii(std::ofstream& file, Subset* subset);
  void Exporter_data_binary(std::ofstream& file, Subset* subset);

private:
  dataFile* data_out;

  //Parametrization
  vector<string> property_type;
  vector<string> property_name;
  vector<int> property_size;
  string format;
  bool is_timestamp;
  bool is_intensity;
  int point_data_idx;
  int point_number;
  int property_number;
  int header_size;
};

#endif
