#ifndef FILE_PCAP_H
#define FILE_PCAP_H

#include "../struct_dataFile.h"

#include "../../common.h"


class file_PCAP
{
public:
  //Constructor / Destructor
  file_PCAP();
  ~file_PCAP();

public:
  vector<dataFile*> Loader(string pathFile);
  vector<dataFile*> Loader(string pathFile, int nb_packet);

  void Loader_vlp16(string pathFile);
  void Loader_hdl32(string pathFile);
  int get_file_length(string pathFile);

  inline void set_lidar_model(string value){this->LiDAR_model = value;}
  inline int* get_packet_max(){return &packet_max;}
  inline int* get_packet_beg(){return &packet_beg;}
  inline int* get_packet_end(){return &packet_end;}

private:
  vector<dataFile*> data_vec;
  string LiDAR_model;
  int packet_max;
  int packet_beg;
  int packet_end;
};

#endif
