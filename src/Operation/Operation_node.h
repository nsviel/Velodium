#ifndef NODE_OPERATION_H
#define NODE_OPERATION_H

#include "Functions/Heatmap.h"
#include "Functions/Selection.h"
#include "Functions/CoordTransform.h"
#include "Transformation/Filter.h"

#include "../Engine/Engine_node.h"
#include "../common.h"


class Operation_node
{
public:
  //Constructor / Destructor
  Operation_node(Engine_node* engine){
    //---------------------------

    this->node_engine = engine;

    //---------------------------
    this->init_objects();
  }
  ~Operation_node();

public:
  //Main functions
  void init_objects(){
    //---------------------------

    this->heatmapManager = new Heatmap();
    this->filterManager = new Filter();
    this->coordManager = new CoordTransform(this);
    this->selectionManager = new Selection(this);

    //---------------------------
  }

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}
  inline CoordTransform* get_coordManager(){return coordManager;}
  inline Selection* get_selectionManager(){return selectionManager;}

private:
  Engine_node* node_engine;

  Heatmap* heatmapManager;
  Filter* filterManager;
  Selection* selectionManager;
  CoordTransform* coordManager;
};

#endif
