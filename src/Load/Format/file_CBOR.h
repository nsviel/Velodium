#ifndef FILE_CBOR_H
#define FILE_CBOR_H

#include "../../Scene/Base/struct_data_file.h"
#include "../../common.h"


class file_CBOR
{
public:
  //Constructor / Destructor
  file_CBOR();
  ~file_CBOR();

public:
  vector<Data_file*> Loader(string pathFile);
  vector<std::uint8_t> readFile(const char* filename);

private:
  //Datatypes
  Data_file* data_out;
};

#endif
