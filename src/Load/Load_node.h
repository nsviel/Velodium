#ifndef NODE_LOAD_H
#define NODE_LOAD_H

#include "Processing/Loader.h"
#include "Processing/Saver.h"
#include "Processing/Pather.h"
#include "Processing/Extractor.h"

#include "../Engine/Engine_node.h"

#include "../common.h"


class Load_node
{
public:
  //Constructor / Destructor
  Load_node(Engine_node* node);
  ~Load_node();

  void update();

public:
  inline Engine_node* get_node_engine(){return node_engine;}
  inline Pather* get_pathManager(){return pathManager;}
  inline Saver* get_saveManager(){return saveManager;}
  inline Loader* get_loadManager(){return loadManager;}
  inline Extractor* get_extractManager(){return extractManager;}

private:
  Engine_node* node_engine;
  Pather* pathManager;
  Saver* saveManager;
  Loader* loadManager;
  Extractor* extractManager;
};

#endif
