#include "Info.h"


int get_file_nbPoint(std::string pathFile){
  //---------------------------

  int cpt=0;
  std::string line;
  std::ifstream infile(pathFile);
  while (std::getline(infile, line)){
    cpt++;
  }

  //---------------------------
  return cpt;
}
float get_file_size(std::string path){
  std::ifstream in(path.c_str(), std::ifstream::ate | std::ifstream::binary);
  float result = (float)in.tellg() / 1000000.0f;
  return result;
}

std::string get_name_from_path(std::string path){
  //---------------------------

  if(path != ""){
    std::string name_format = path.substr(path.find_last_of("/\\") + 1);
    return name_format.substr(0, name_format.find_last_of("."));
  }else{
    return "";
  }

  //---------------------------
}
std::string get_format_from_path(std::string path){
  //---------------------------

  if(path != ""){
    std::string name_format = path.substr(path.find_last_of("/\\") + 1);
    return name_format.substr(name_format.find_last_of(".") + 1, std::string::npos);
  }else{
    return "";
  }

  //---------------------------
}
std::string get_name_and_parent_from_path(std::string path){
  if(path != ""){
    std::string path_parent = path.substr(0, path.find_last_of("/\\"));
    std::string parent = path_parent.substr(path_parent.find_last_of("/\\"));
    std::string name_format = path.substr(path.find_last_of("/\\"));
    std::string parent_name_format = parent + name_format;
    return parent_name_format.substr(0, parent_name_format.find_last_of("."));
  }else{
    return "";
  }
}
std::string get_filename_from_path(std::string path){
  if(path != ""){
    std::string name_format = path.substr(path.find_last_of("/\\") + 1);
    return name_format;
  }else{
    return "";
  }
}
std::string get_type_from_path(std::string path){
  std::string type;
  if(std::filesystem::is_directory(path)){
    type = "Folder";
  }else{
    type = "File";
  }
  return type;
}
std::string get_format_from_filename(std::string path){
  //---------------------------

  std::string format = path.substr(path.find_last_of("."), std::string::npos);

  //---------------------------
  return format;
}
