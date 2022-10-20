#ifdef WITH_PCL
#ifndef FILE_PCD_H
#define FILE_PCD_H

#include "../../common.h"
#include "../../Engine/Data/struct_dataFile.h"

#include <pcl/io/pcd_io.h>

class filePCD
{
public:
  //Constructor / Destructor
  filePCD();
  ~filePCD();

public:
  dataFile* Loader(string pathFile);
  string Loader_header(string pathFile);
  void Loader_XYZ(string pathFile);
  void Loader_XYZI(string pathFile);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
#endif
