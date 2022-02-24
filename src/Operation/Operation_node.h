#ifndef NODE_OPERATION_H
#define NODE_OPERATION_H

#include "Functions/Heatmap.h"
#include "Functions/Selection.h"
#include "Functions/CoordTransform.h"
#include "Functions/Extraction.h"
#include "Transformation/Filter.h"
#include "Transformation/Attribut.h"
#include "Optimization/Fitting.h"

#include "../Engine/Engine_node.h"
#include "../common.h"


class Operation_node
{
public:
  //Constructor / Destructor
  Operation_node(Engine_node* engine){
    this->node_engine = engine;
    //---------------------------

    this->fittingManager = new Fitting(this);
    this->attribManager = new Attribut(this);
    this->heatmapManager = new Heatmap(this);
    this->filterManager = new Filter(this);
    this->coordManager = new CoordTransform(this);
    this->selectionManager = new Selection(this);
    this->extractionManager = new Extraction(this);

    //---------------------------
  }
  ~Operation_node();

public:

  inline Engine_node* get_node_engine(){return node_engine;}
  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}
  inline CoordTransform* get_coordManager(){return coordManager;}
  inline Selection* get_selectionManager(){return selectionManager;}
  inline Attribut* get_attribManager(){return attribManager;}
  inline Extraction* get_extractionManager(){return extractionManager;}
  inline Fitting* get_fittingManager(){return fittingManager;}

private:
  Engine_node* node_engine;

  Attribut* attribManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Selection* selectionManager;
  CoordTransform* coordManager;
  Extraction* extractionManager;
  Fitting* fittingManager;
};

#endif
