#ifndef FILE_CSV_H
#define FILE_CSV_H

#include "../CSV/CSV_parser.h"

#include "../../common.h"
#include "../../Engine/Data/struct_dataFile.h"


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
