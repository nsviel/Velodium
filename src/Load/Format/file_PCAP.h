#ifndef FILE_PCAP_H
#define FILE_PCAP_H

#include "../struct_dataFile.h"

#include "../../common.h"


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
