#include "UDP_server.h"

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

  this->packet_size = 1206;
  this->is_binded = false;

  //---------------------------
}
UDP_server::~UDP_server(){}

//Main function
vector<int> UDP_server::read_UDP_packets(int port){
  //---------------------------

  this->server_binding(port);
  this->server_read_data();

  //---------------------------
  return packet_dec;
}

//Subfunctions
void UDP_server::server_binding(int port){
  if(is_binded == false){
    //---------------------------

    // Creating socket file descriptor
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(sock < 0){
      cout << "socket creation failed" << endl;
      exit(EXIT_FAILURE);
    }

    // Filling server information
    sockaddr_in addr;
    addr.sin_family    = AF_INET; // IPv4
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    // Bind the socket with the server address
    int binding = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    //Check final success
    if(binding >= 0){
      this->is_binded = true;
    }else{
      cout << "bind failed" << endl;
      this->is_binded = false;
    }

    //---------------------------

  }
}
void UDP_server::server_unbinding(){
  //---------------------------

  this->is_binded = false;
  close(sock);

  //---------------------------
}
void UDP_server::server_read_data(){
  char buffer[packet_size] = {0};
  sockaddr_in from;
  socklen_t fromlen = sizeof(from);
  //---------------------------

  //The thread blocks here until a packet is received (MSG_WAITALL)
  int udp_size = recvfrom(sock, buffer, packet_size, MSG_WAITALL, reinterpret_cast<sockaddr*>(&from), &fromlen);

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
