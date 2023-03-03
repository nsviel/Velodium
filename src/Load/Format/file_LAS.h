#ifndef FILE_LAS_H
#define FILE_LAS_H

#include "../../Scene/Base/struct_data_file.h"
#include "../../common.h"


class file_LAS
{
public:
  //Constructor / Destructor
  file_LAS();
  ~file_LAS();

public:
  //Main function
  Data_file* Loader(string filePath);
  void read(string path);

private:
  //Datatypes
  Data_file* data_out;
};

#endif
