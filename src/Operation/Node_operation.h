#ifndef NODE_OPERATION_H
#define NODE_OPERATION_H

#include "../common.h"

class Node_engine;
class Node_load;
class Node_gui;

class Attribut;
class Heatmap;
class Filter;
class Selection;
class CoordTransform;
class Extraction;
class Fitting;
class Color;
class Player;
class Online;
class Saving;

class GUI_Color;
class GUI_Online;
class GUI_Player;


class Node_operation
{
public:
  //Constructor / Destructor
  Node_operation(Node_engine* engine);
  ~Node_operation();

public:
  void update();
  void runtime();
  void draw();

  inline Node_engine* get_node_engine(){return node_engine;}
  inline Node_load* get_node_load(){return node_load;}
  inline Node_gui* get_node_gui(){return node_gui;}

  inline Heatmap* get_heatmapManager(){return heatmapManager;}
  inline Filter* get_filterManager(){return filterManager;}
  inline CoordTransform* get_coordManager(){return coordManager;}
  inline Selection* get_selectionManager(){return selectionManager;}
  inline Attribut* get_attribManager(){return attribManager;}
  inline Extraction* get_extractionManager(){return extractionManager;}
  inline Fitting* get_fittingManager(){return fittingManager;}
  inline Color* get_colorManager(){return colorManager;}
  inline Online* get_onlineManager(){return onlineManager;}
  inline Player* get_playerManager(){return playerManager;}
  inline Saving* get_savingManager(){return savingManager;}

  inline GUI_Color* get_gui_color(){return gui_color;}
  inline GUI_Online* get_gui_online(){return gui_online;}
  inline GUI_Player* get_gui_player(){return gui_player;}

private:
  Node_engine* node_engine;
  Node_load* node_load;
  Node_gui* node_gui;

  Attribut* attribManager;
  Heatmap* heatmapManager;
  Filter* filterManager;
  Selection* selectionManager;
  CoordTransform* coordManager;
  Extraction* extractionManager;
  Fitting* fittingManager;
  Color* colorManager;
  Online* onlineManager;
  Player* playerManager;
  Saving* savingManager;

  GUI_Color* gui_color;
  GUI_Online* gui_online;
  GUI_Player* gui_player;
};

#endif
