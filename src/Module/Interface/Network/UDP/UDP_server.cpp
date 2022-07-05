#include "UDP_server.h"

#include "../Socket/Socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <bitset>


//Constructor / Destructor
UDP_server::UDP_server(){
  //---------------------------

  this->sock_server = new Socket();
  this->sock_client = new Socket();

  //---------------------------
}
UDP_server::~UDP_server(){}

//Socket function
void UDP_server::capture_init(int port, int packet_size){
  bool is_binded = sock_server->get_is_binded();
  //---------------------------

  if(sock_server->get_is_binded()){
    capture_stop();
  }

  sock_server->socket_binding(port, packet_size);
  sock_client->socket_binding(port, 256);

  //---------------------------
}
void UDP_server::capture_stop(){
  //---------------------------

  if(sock_server->get_is_binded()){
    sock_server->socket_disconnect();
    sock_server->socket_disconnect();
  }

  //---------------------------
}
vector<int> UDP_server::capture_packet(){
  vector<int> data_dec;
  //---------------------------

  if(sock_server->get_is_binded()){
    sock_server->socket_recv_data();
    data_dec = sock_server->get_data_dec();
  }

  //---------------------------
  return data_dec;
}
