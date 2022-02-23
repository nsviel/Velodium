#ifndef FILE_CSV_H
#define FILE_CSV_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"


class file_CSV
{
public:
  //Constructor / Destructor
  file_CSV();
  ~file_CSV();

public:
  vector<dataFile*> Loader(string pathFile);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
