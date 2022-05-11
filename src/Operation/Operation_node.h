#ifndef NODE_OPERATION_H
#define NODE_OPERATION_H

#include "../common.h"

class Engine_node;
class Attribut;
class Heatmap;
class Filter;
class Selection;
class CoordTransform;
class Extraction;
class Fitting;
class Color;


class Operation_node
{
public:
  //Constructor / Destructor
  Operation_node(Engine_node* engine);
  ~Operation_node();

public:
  void update();
  
  inline Engine_node* get_node_engine(){return node_engine;}
  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}
  inline CoordTransform* get_coordManager(){return coordManager;}
  inline Selection* get_selectionManager(){return selectionManager;}
  inline Attribut* get_attribManager(){return attribManager;}
  inline Extraction* get_extractionManager(){return extractionManager;}
  inline Fitting* get_fittingManager(){return fittingManager;}
  inline Color* get_colorManager(){return colorManager;}

private:
  Engine_node* node_engine;

  Attribut* attribManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Selection* selectionManager;
  CoordTransform* coordManager;
  Extraction* extractionManager;
  Fitting* fittingManager;
  Color* colorManager;
};

#endif
