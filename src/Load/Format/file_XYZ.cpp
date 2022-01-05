#include "file_XYZ.h"

#include <fstream>


//Constructor / Destructor
file_XYZ::file_XYZ(){}
file_XYZ::~file_XYZ(){}

//Main function
dataFile* file_XYZ::Loader(string filePath){
  dataFile* data = new dataFile();
  //---------------------------

  //Open file
  std::ifstream infile(filePath);

  //Retrieve data
  std::string line;
  float a, b, c, d, e, f;
  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> a >> b >> c >> d >> e >> f;

    //Data extraction
    data->location.push_back(vec3(a, b, c));
    data->color.push_back(vec4(d, e, f, 1));
  }

  //---------------------------
  return data;
}
