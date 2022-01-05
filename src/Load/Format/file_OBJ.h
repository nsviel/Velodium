#ifndef FILE_OBJ_H
#define FILE_OBJ_H

#include "../struct_dataFile.h"

#include "../../common.h"


class file_OBJ
{
public:
  //Constructor / Destructor
  file_OBJ();
  ~file_OBJ();

public:
  dataFile* Loader(string filePath);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
