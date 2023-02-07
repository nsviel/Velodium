#ifndef FILE_CSV_H
#define FILE_CSV_H

#include "../../Engine/Data/struct_data_file.h"
#include "../../common.h"


class file_CSV
{
public:
  //Constructor / Destructor
  file_CSV();
  ~file_CSV();

public:
  vector<Data_file*> Loader(string pathFile);

private:
  //Datatypes
  Data_file* data_out;
};

#endif
