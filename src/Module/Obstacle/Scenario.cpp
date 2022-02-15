#include "Scenario.h"

#include "Interfacing.h"

#include "../Module_node.h"

//TODO: make explicit scenario state for each element
//TODO: Build scenarios parameters


//Constructor / Destructor
Scenario::Scenario(Module_node* node_module){
  //---------------------------

  this->ioManager = node_module->get_ioManager();

  this->scenario_selected = 1;
  this->scenario_started = false;

  //---------------------------
}
Scenario::~Scenario(){}

void Scenario::scenario_start(){
  //---------------------------

  switch(scenario_selected){
    case 0:{//WP4 auto
      this->scenario_WP4_auto();
      break;
    }
    case 1:{//WP5 train on-board
      this->scenario_WP5_train_board();
      break;
    }
    case 2:{//WP5 train on-edge
      this->scenario_WP5_train_edge();
      break;
    }
  }

  //---------------------------
}
void Scenario::scenario_stop(){
  //---------------------------


  //---------------------------
}

void Scenario::scenario_common(){
  //---------------------------

  ioManager->start_dirWatcher();

  //---------------------------
}
void Scenario::scenario_WP4_auto(){
  //---------------------------

  this->scenario_common();


  //---------------------------
}
void Scenario::scenario_WP5_train_board(){
  //---------------------------

  this->scenario_common();

  //---------------------------
}
void Scenario::scenario_WP5_train_edge(){
  //---------------------------

  this->scenario_common();

  //---------------------------
}
