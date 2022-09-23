#ifndef FILE_CBOR_H
#define FILE_CBOR_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"


class file_CBOR
{
public:
  //Constructor / Destructor
  file_CBOR();
  ~file_CBOR();

public:
  vector<dataFile*> Loader(string pathFile);
  vector<std::uint8_t> readFile(const char* filename);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
