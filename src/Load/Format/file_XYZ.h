#ifndef FILE_XYZ_H
#define FILE_XYZ_H

#include "../../Engine/Data/struct_data_file.h"
#include "../../common.h"


class file_XYZ
{
public:
  //Constructor / Destructor
  file_XYZ();
  ~file_XYZ();

public:
  Data_file* Loader(string filePath);

private:
  //Datatypes
  Data_file* data_out;
};

#endif
