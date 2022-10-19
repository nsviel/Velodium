#ifndef NODE_LOAD_H
#define NODE_LOAD_H

#include "Processing/Loader.h"
#include "Processing/Saver.h"
#include "Processing/Pather.h"
#include "Processing/Extractor.h"

#include "../Engine/Node_engine.h"

#include "../common.h"


class Node_load
{
public:
  //Constructor / Destructor
  Node_load(Node_engine* node);
  ~Node_load();

  void update();

public:
  inline Node_engine* get_node_engine(){return node_engine;}
  inline Pather* get_pathManager(){return pathManager;}
  inline Saver* get_saveManager(){return saveManager;}
  inline Loader* get_loadManager(){return loadManager;}
  inline Extractor* get_extractManager(){return extractManager;}

private:
  Node_engine* node_engine;
  Pather* pathManager;
  Saver* saveManager;
  Loader* loadManager;
  Extractor* extractManager;
};

#endif
