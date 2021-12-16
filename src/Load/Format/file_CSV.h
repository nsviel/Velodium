#ifndef FILE_CSV_H
#define FILE_CSV_H

#include "../struct_dataFile.h"

#include "../../common.h"


class fileCSV
{
public:
  //Constructor / Destructor
  fileCSV();
  ~fileCSV();

public:
  vector<dataFile*> Loader(string pathFile);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
