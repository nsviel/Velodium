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
  vector<string> zenity_loading(string title);
  string zenity_saving(string filename, string& path_current_dir);
  string zenity_directory(string& path_current_dir);
  void zenity_select_directory(string& path_dir);

private:
};

#endif
