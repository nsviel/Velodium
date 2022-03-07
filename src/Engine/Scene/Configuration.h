#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "../../common.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>
#include <fstream>


class Configuration
{
public:
  //Constructor / Destructor
  Configuration();
  ~Configuration();

public:
  //Main functions
  void make_configuration();
  void make_preconfig(int config);

  //Pred-defined configurations
  void preconf_default(Json::Value& root);
  void preconf_AI_module(Json::Value& root);
  void preconf_WP4_car(Json::Value& root);
  void preconf_WP5_train(Json::Value& root);

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
  string path_default;
  string path_WP4_car;
  string path_WP5_train;
  string path_AI;

  int config;
};

#endif
