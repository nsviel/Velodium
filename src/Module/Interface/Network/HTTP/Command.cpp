#include "Command.h"

#include <fstream>


//Constructor / Destructor
Command::Command(){
  //---------------------------


  //---------------------------
}
Command::~Command(){}

// GET request handlers
vector<vector<string>> Command::parse_http_config(){
  string path = "../media/engine/http_conf.txt";
  //---------------------------

  //Read file
  string line;
  vector<vector<string>> opt_vec;
  std::ifstream file_conf(path);
  int cpt = 0;
  while(std::getline(file_conf, line)){
    if(line.empty()){
      break;
    }

    std::istringstream iss(line);
    string opt, val;
    iss >> opt >> val;

    vector<string> option;
    option.push_back(opt);
    option.push_back(val);
    opt_vec.push_back(option);
  }

  //Empty file
  ofstream file;
  file.open(path);
  file << "";
  file.close();

  //---------------------------
  return opt_vec;
}
