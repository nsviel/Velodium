#ifndef GUI_OPERATION_H
#define GUI_OPERATION_H

#include "Color/GUI/GUI_Color.h"

#include "../GUI/GUI_node.h"
#include "../common.h"


class GUI_operation
{
public:
  //Constructor / Destructor
  GUI_operation(GUI_node* node_gui){
    //-------------------------------

    this->gui_color = new GUI_Color(node_gui);

    //-------------------------------
  }
  ~GUI_operation();

public:
  inline GUI_Color* get_gui_color(){return gui_color;}

private:
  GUI_Color* gui_color;
};

#endif
