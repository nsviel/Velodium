#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "../common.h"

class Configuration;
class Node_engine;
class Loader;


class Argument
{
public:
  //Constructor / Destructor
  Argument(Node_engine* engine);
  ~Argument();

public:
  void process_arg(int argc, char* argv[]);

private:
  Configuration* configManager;
  Loader* loadManager;
};

#endif
