#ifndef CONFIG_MODULE_H
#define CONFIG_MODULE_H

#include "../../common.h"


class config_module
{
public:
  //Constructor / Destructor
  config_module();
  ~config_module();

public:
  //Main functions
  void make_configuration();
  void choose_configuration(int config);

  //json stuff
  void create_jsonfile_default();
  void update_jsonfile(string field, string title, string value);
  bool is_file_exist(string fileName);

  float parse_json_f(string field, string value);
  string parse_json_s(string field, string value);
  int parse_json_i(string field, string value);
  bool parse_json_b(string field, string value);

private:
  string path_file;
};

#endif
