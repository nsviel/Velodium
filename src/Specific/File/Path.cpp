#include "Path.h"

#include <limits.h>
#include <stdlib.h>


std::string get_path_abs_build(){
  //---------------------------

  std::string absPath = std::experimental::filesystem::current_path();
  absPath += "/";

  //---------------------------
  return absPath;
}
std::string get_path_abs(std::string path){
  //---------------------------

  std::string path_abs = realpath(path.c_str(), NULL);

  //---------------------------
  return path_abs;
}
