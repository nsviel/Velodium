#ifndef NODE_OPERATION_H
#define NODE_OPERATION_H

#include "Functions/Heatmap.h"
#include "Transformation/Filter.h"

#include "../common.h"


class Operation_node
{
public:
  //Constructor / Destructor
  Operation_node(){
    this->init_objects();
  }
  ~Operation_node();

public:
  //Main functions
  void init_objects(){
    this->heatmapManager = new Heatmap();
    this->filterManager = new Filter();
  }

  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}

private:
  Heatmap* heatmapManager;
  Filter* filterManager;
};

#endif
