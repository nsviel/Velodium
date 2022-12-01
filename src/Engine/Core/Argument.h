#ifndef ARGUMENT_H
#define ARGUMENT_H

#include "../../common.h"

class Configuration;
class Node_engine;
class Loader;
class Camera;


class Argument
{
public:
  //Constructor / Destructor
  Argument(Node_engine* node);
  ~Argument();

public:
  void process_arg(int argc, char* argv[]);

private:
  Node_engine* node_engine;
  Configuration* configManager;
  Loader* loadManager;
  Camera* cameraManager;
};

#endif
