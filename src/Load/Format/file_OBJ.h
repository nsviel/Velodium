#ifndef FILE_OBJ_H
#define FILE_OBJ_H

#include "../../common.h"
#include "../../Engine/Data/struct_dataFile.h"


class fileOBJ
{
public:
  //Constructor / Destructor
  fileOBJ();
  ~fileOBJ();

public:
  dataFile* Loader(string filePath);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
