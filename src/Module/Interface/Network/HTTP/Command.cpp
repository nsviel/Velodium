#include "Command.h"

#include "../Network.h"
#include "../../Interface_node.h"
#include "../../../Module_node.h"
#include "../../../Player/Player_node.h"
#include "../../../Player/Dynamic/Followup.h"
#include "../../../Player/Dynamic/Online.h"


//Constructor / Destructor
Command::Command(Network* netManager){
  //---------------------------

  Interface_node* node_interface = netManager->get_node_interface();
  Module_node* node_module = node_interface->get_node_module();

  this->node_player = node_module->get_node_player();
  Online* onlineManager = node_player->get_onlineManager();
  Followup* followManager = node_player->get_followManager();

  //---------------------------
}
Command::~Command(){}

// GET request handlers
void Command::http_get_slam_on(){
  //---------------------------



  say("slam on");

  //---------------------------
}
void Command::http_get_slam_off(){
  //---------------------------

  say("slam off");

  //---------------------------
}
void Command::http_get_view_top(){
  //---------------------------

  say("view top");

  //---------------------------
}
void Command::http_get_view_oblique(){
  //---------------------------

  say("view oblique");

  //---------------------------
}
