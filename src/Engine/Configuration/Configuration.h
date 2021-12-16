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

  //json stuff
  void create_jsonfile();
  void update_jsonfile(string field, string title, string value);
  float parse_json_float(string field, string value);
  string parse_json_string(string field, string value);
  int parse_json_int(string field, string value);
  bool parse_json_bool(string field, string value);
  bool is_file_exist(string fileName);

private:
  string jsonPath;

};

#endif
