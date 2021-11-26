#ifndef FILE_PCAP_H
#define FILE_PCAP_H

#include "../../common.h"

#include "../../Engine/Data/struct_dataFile.h"
#include "../../Module/Velodyne/UDP/struct_UDPpacket.h"



class filePCAP
{
public:
  //Constructor / Destructor
  filePCAP();
  ~filePCAP();

public:

  vector<dataFile*> Loader(string pathFile);

private:
  vector<dataFile*> data_vec;
  bool vlp16, hdl32;
};

#endif
