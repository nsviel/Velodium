#ifndef COMMAND_H
#define COMMAND_H

#include "../../../../common.h"


class Command
{
public:
  Command();
  ~Command();

public:
  vector<vector<string>> parse_http_config();

private:
};

#endif
