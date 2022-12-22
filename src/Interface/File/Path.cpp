#include "Path.h"


std::string get_absolutePath_build(){
  //---------------------------

  std::string absPath = std::experimental::filesystem::current_path();
  absPath += "/";

  //---------------------------
  return absPath;
}
