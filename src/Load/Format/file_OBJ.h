#ifndef FILE_OBJ_H
#define FILE_OBJ_H

#include "../struct_dataFile.h"

#include "../../common.h"


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
