#ifndef NODE_GUI_H
#define NODE_GUI_H

#include "../common.h"


class GUI_node
{
public:
  //Constructor / Destructor
  GUI_node(Engine_node* engine, Operation_node* ope){
    //---------------------------

    this->node_engineManager = engine;
    this->node_opeManager = ope;
    this->init_objects();

    //---------------------------
  }
  ~GUI_node(){}

public:
  //Main functions
  void init_objects(){
    //---------------------------



    //---------------------------
  }
  void reset(){
    //---------------------------

    //---------------------------
  }
  void update(){}
  void runtime(){
    //---------------------------

    //---------------------------
  }

  inline Engine_node* get_node_engineManager(){return node_engineManager;}
  inline Operation_node* get_node_opeManager(){return node_opeManager;}

private:
  Engine_node* node_engineManager;
  Operation_node* node_opeManager;
};

#endif
