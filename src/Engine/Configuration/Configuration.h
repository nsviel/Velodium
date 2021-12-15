#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../../common.h"


class Configuration
{
public:
  //Constructor / Destructor
  Configuration();
  ~Configuration();

public:
  //Main functions
  void make_configuration();
  void save_configuration();

  //Subfunctions
  void initialize_configStruct();
  void create_configFile();
  void write_configData();
  void read_configData();
  bool is_file_exist(string fileName);

  //json stuff
  void create_jsonfile();
  float parse_json_float(string field, string value);
  string parse_json_string(string field, string value);
  int parse_json_int(string field, string value);
  bool parse_json_bool(string field, string value);


private:
  string configFilePath;
  string jsonPath;

};

#endif
