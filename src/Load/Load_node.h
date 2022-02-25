#ifndef NODE_LOAD_H
#define NODE_LOAD_H

#include "Processing/Loader.h"
#include "Processing/Saver.h"
#include "Processing/Pather.h"
#include "Processing/dataExtraction.h"

#include "../Engine/Engine_node.h"

#include "../common.h"


class Load_node
{
public:
  //Constructor / Destructor
  Load_node(Engine_node* engine){
    this->node_engine = engine;
    //---------------------------

    this->extractManager = new dataExtraction(this);
    this->saveManager = new Saver(this);
    this->loadManager = new Loader(this);
    this->pathManager = new Pather(this);

    //---------------------------
  }
  ~Load_node();

public:
  inline Engine_node* get_node_engine(){return node_engine;}
  inline Pather* get_pathManager(){return pathManager;}
  inline Saver* get_saveManager(){return saveManager;}
  inline Loader* get_loadManager(){return loadManager;}
  inline dataExtraction* get_extractManager(){return extractManager;}

private:
  Engine_node* node_engine;
  Pather* pathManager;
  Saver* saveManager;
  Loader* loadManager;
  dataExtraction* extractManager;
};

#endif
