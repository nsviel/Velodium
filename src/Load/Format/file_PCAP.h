#ifndef FILE_PCAP_H
#define FILE_PCAP_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"


class file_PCAP
{
public:
  //Constructor / Destructor
  file_PCAP();
  ~file_PCAP();

public:
  vector<dataFile*> Loader(string pathFile);

  void Loader_vlp16(string pathFile);
  void Loader_hdl32(string pathFile);
  int get_file_length(string pathFile);

  inline void set_lidar_model(string value){this->LiDAR_model = value;}
  inline bool* get_packet_range_on(){return &packet_range_on;}
  inline int* get_packet_beg(){return &packet_beg;}
  inline int* get_packet_end(){return &packet_end;}

private:
  vector<dataFile*> data_vec;
  string LiDAR_model;
  bool packet_range_on;
  int packet_beg;
  int packet_end;
};

#endif
