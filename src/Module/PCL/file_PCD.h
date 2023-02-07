#ifdef WITH_PCL
#ifndef FILE_PCD_H
#define FILE_PCD_H

#include "../../common.h"
#include "../../Engine/Data/struct_data_file.h"

#include <pcl/io/pcd_io.h>

class filePCD
{
public:
  //Constructor / Destructor
  filePCD();
  ~filePCD();

public:
  Data_file* Loader(string pathFile);
  string Loader_header(string pathFile);
  void Loader_XYZ(string pathFile);
  void Loader_XYZI(string pathFile);

private:
  //Datatypes
  Data_file* data_out;
};

#endif
#endif
