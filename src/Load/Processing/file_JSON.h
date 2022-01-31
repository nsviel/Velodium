#ifndef FILE_JSON_H
#define FILE_JSON_H

#include "../../common.h"


class file_JSON
{
public:
  //Constructor / Destructor
  file_JSON();
  ~file_JSON();

public:
  void parse_json_obstacle(Cloud* cloud, vector<string> paths, string data);

private:

};

#endif
