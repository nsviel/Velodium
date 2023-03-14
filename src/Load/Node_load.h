#ifndef NODE_LOAD_H
#define NODE_LOAD_H

#include "Processing/Loader.h"
#include "Processing/Saver.h"
#include "Processing/Pather.h"
#include "Processing/Extractor.h"

#include "../Engine/Node_engine.h"
#include "../Specific/Base/Node.h"


class Node_load : public Node
{
public:
  //Constructor / Destructor
  Node_load(Node_engine* node);
  ~Node_load();

  void update();

public:
  inline Node_engine* get_node_engine(){return node_engine;}
  inline Pather* get_patherManager(){return patherManager;}
  inline Saver* get_saverManager(){return saverManager;}
  inline Loader* get_loaderManager(){return loaderManager;}

private:
  Node_engine* node_engine;
  Pather* patherManager;
  Saver* saverManager;
  Loader* loaderManager;
  Extractor* extractManager;
};

#endif
