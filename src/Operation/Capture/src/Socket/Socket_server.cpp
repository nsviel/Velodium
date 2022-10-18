#include "Socket_server.h"

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
Socket_server::Socket_server(){
  //---------------------------

  this->is_binded = false;

  //---------------------------
}
Socket_server::~Socket_server(){}

//Socket function
void Socket_server::socket_binding(int port_server, int packet_size_max){
  if(is_binded == false){
    this->port = port_server;
    this->packet_size = packet_size_max;
    //---------------------------

    // Creating socket file descriptor
    this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0){
      cout << "socket creation failed" << endl;
      exit(EXIT_FAILURE);
    }

    // Filling server information
    sockaddr_in addr;
    addr.sin_family       = AF_INET; // IPv4
    addr.sin_addr.s_addr  = INADDR_ANY;
    addr.sin_port         = htons(port);

    // Bind the socket with the server address
    int binding = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    //Check final success
    if(binding == 0){
      this->is_binded = true;
    }else{
      cout << "[error] Socket binding failed for port [" << port << "]" << endl;
      this->is_binded = false;
    }

    //---------------------------
  }
}
void Socket_server::socket_recv_data(){
  //---------------------------

  //Parameter
  char buffer[packet_size] = {0};
  sockaddr_in addr;
  addr.sin_family    = AF_INET; // IPv4
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);
  unsigned int length = sizeof(addr);

  //Thread blocking: MSG_DONTWAIT / MSG_WAITALL
  int udp_size = recvfrom(sock, buffer, packet_size, MSG_WAITALL, reinterpret_cast<sockaddr*>(&addr), &length);

  //Once packet received, process it
  packet_dec.clear();
  if(udp_size != 0 && udp_size != 512){
    for(int i=0; i<udp_size; i++){
      bitset<8> octet(buffer[i]);

      int octet_32 = octet.to_ulong();
      packet_dec.push_back(octet_32);
    }
  }

  //---------------------------
}
void Socket_server::socket_disconnect(){
  //---------------------------

  if(is_binded){
    close(sock);
    this->is_binded = false;
  }

  //---------------------------
}
