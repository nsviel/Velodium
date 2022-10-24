#ifndef ZENITY_H
#define ZENITY_H

#include "../../common.h"


class Zenity
{
public:
  //Constructor / Destructor
  Zenity();
  ~Zenity();

public:
  //Zenity function
  vector<string> zenity_loading(string& path_current_dir, string title);
  string zenity_saving(string& path_current_dir, string filename);
  string zenity_directory(string& path_current_dir);
  void zenity_select_directory(string& path_dir);

private:
};

#endif
