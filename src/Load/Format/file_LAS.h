#ifndef FILE_LAS_H
#define FILE_LAS_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"


class file_LAS
{
public:
  //Constructor / Destructor
  file_LAS();
  ~file_LAS();

public:
  //Main function
  dataFile* Loader(string filePath);


private:
  //Datatypes
  dataFile* data_out;
};

#endif
