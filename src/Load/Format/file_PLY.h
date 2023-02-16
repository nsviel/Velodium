#ifndef FILE_PLY_H
#define FILE_PLY_H

#include "../../Engine/Data/struct_data_file.h"
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
  Data_file* Loader(string path_file);
  bool Exporter_cloud(string path_file, string format, Cloud* cloud);
  bool Exporter_subset(string path_dir, string format, Subset* subset);
  bool Exporter_subset(string path_dir, string format, Subset* subset, string fileName);
  bool Exporter_set(string path_dir, string ply_format, Cloud* cloud, int ID, int nb);

private:
  //Loader data
  void Loader_header(std::ifstream& file);
  void Loader_ascii(std::ifstream& file);
  void Loader_ascii_withface(std::ifstream& file);
  void Loader_bin_little_endian(std::ifstream& file);
  void Loader_bin_little_endian_withface(std::ifstream& file);
  void Loader_bin_big_endian(std::ifstream& file);
  void Loader_bin_big_endian_withface(std::ifstream& file);

  //Loader subfunctions
  float reverse_float(const float inFloat);
  int reverse_int(const int inInt);
  void reorder_by_timestamp();
  int get_id_property(string name);
  float get_float_from_binary(char* data, int& offset);
  float get_double_from_binary(char* block_data, int& offset);
  float get_int_from_binary(char* data, int& offset);
  float get_uint8_from_binary(char* block_data, int& offset);
  float get_uint16_from_binary(char* block_data, int& offset);
  float get_uint32_from_binary(char* block_data, int& offset);
  float get_uchar_from_binary(char* block_data, int& offset);

  //Exporter subfunctions
  void Exporter_header(std::ofstream& file, string format, Subset* subset, int nb_point);
  void Exporter_data_ascii(std::ofstream& file, Subset* subset);
  void Exporter_data_binary(std::ofstream& file, Subset* subset);

private:
  Data_file* data_out;

  //Parametrization
  vector<string> property_type;
  vector<string> property_name;
  vector<int> property_size;
  string format;
  bool is_timestamp;
  bool is_intensity;
  bool is_normal;
  bool is_color;
  int point_data_idx;
  int point_number;
  int face_number;
  int property_number;
  int header_size;
};

#endif
