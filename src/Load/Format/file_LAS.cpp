#include "file_LAS.h"
#include "../../Specific/fct_system.h"

#include <fstream>



//Constructor / Destructor
file_LAS::file_LAS(){}
file_LAS::~file_LAS(){}

//Main function
dataFile* file_LAS::Loader(string filePath){
  ifstream infile(filePath);
  dataFile* data = new dataFile();
  data->name = get_name_from_path(filePath);
  data->path = filePath;
  //---------------------------



  //---------------------------
  return data;
}
