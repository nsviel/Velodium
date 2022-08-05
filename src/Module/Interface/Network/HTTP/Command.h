#ifndef COMMAND_H
#define COMMAND_H

#include "../../../../common.h"

class Network;
class Player_node;


class Command
{
public:
  Command(Network* netManager);
  ~Command();

public:
  //Get request functions
  static void http_get_slam_on();
  static void http_get_slam_off();
  static void http_get_view_top();
  static void http_get_view_oblique();

private:
  Player_node* node_player;
};

#endif
