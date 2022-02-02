#ifndef CONFIG_OPENGL_H
#define CONFIG_OPENGL_H

#include "../../common.h"


class config_opengl
{
public:
  //Constructor / Destructor
  config_opengl();
  ~config_opengl();

public:
  //Main functions
  void make_configuration();

  //json stuff
  void create_jsonfile();
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
