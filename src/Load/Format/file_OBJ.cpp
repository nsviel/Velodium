#include "file_OBJ.h"

#include <fstream>


//Constructor / Destructor
file_OBJ::file_OBJ(){}
file_OBJ::~file_OBJ(){}

//Main function
dataFile* file_OBJ::Loader(string filePath){
  std::ifstream infile(filePath);
  std::string line;
  float a, b, c;
  string ID;
  dataFile* data = new dataFile();
  //---------------------------

  float R = float(rand()%101)/100;
  float G = float(rand()%101)/100;
  float B = float(rand()%101)/100;

  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> ID >> a >> b >> c;

    //Data extraction
    if(ID == "v" ) data->location.push_back(vec3(a, b, c));
    if(ID == "vn") data->normal.push_back(vec3(a, b, c));
  }

  //---------------------------
  return data;
}
