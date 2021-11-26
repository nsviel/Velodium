#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <fstream>

/**
 * \file Configuration.h
 * \brief Configuration file management
 * \author Nathan Sanchiz-Viel
 *
 * Keep information in an external file about dimensions, folder paths and so on
 *
 */

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

private:
  string configFilePath;

};

#endif
