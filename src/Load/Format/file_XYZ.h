#ifndef FILE_XYZ_H
#define FILE_XYZ_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"


class file_XYZ
{
public:
  //Constructor / Destructor
  file_XYZ();
  ~file_XYZ();

public:
  dataFile* Loader(string filePath);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
