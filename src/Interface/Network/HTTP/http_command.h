#ifndef HTTP_COMMAND_H
#define HTTP_COMMAND_H

#include "../../../common.h"


class http_command
{
public:
  http_command();
  ~http_command();

public:
  vector<vector<string>> parse_http_config();

private:
};

#endif
