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
  bool is_file_exist(string fileName);

  //Basic parsing accesseurs
  float parse_json_f(string field, string value);
  string parse_json_s(string field, string value);
  int parse_json_i(string field, string value);
  bool parse_json_b(string field, string value);

private:
  string path_config;
};

#endif
