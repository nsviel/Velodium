#ifndef FILE_XYZ_H
#define FILE_XYZ_H

#include "../../common.h"
#include "../../Engine/Data/struct_dataFile.h"


class fileXYZ
{
public:
  //Constructor / Destructor
  fileXYZ();
  ~fileXYZ();

public:
  dataFile* Loader(string filePath);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
