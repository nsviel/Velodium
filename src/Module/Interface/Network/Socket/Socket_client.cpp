#include "Socket_client.h"

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
Socket_client::Socket_client(){
  //---------------------------

  this->is_binded = false;

  //---------------------------
}
Socket_client::~Socket_client(){}

//Socket function
void Socket_client::socket_binding(int port_sock, int packet_size_){
  if(is_binded == false){
    this->port = port_sock;
    this->packet_size = packet_size_;
    //---------------------------

    // Creating socket file descriptor
    this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
    if(binding == 0){
      this->is_binded = true;
    }else{
      cout << "[error] Socket binding failed for port [" << port << "]" << endl;
      this->is_binded = false;
    }

    //---------------------------
  }
}
void Socket_client::socket_send_data(){
  //---------------------------

  //---------------------------
}
void Socket_client::socket_recv_data(){
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

  //Check for connection test number
  this->code = compute_recv_code(packet_dec);

  //---------------------------
}
void Socket_client::socket_disconnect(){
  //---------------------------

  if(is_binded){
    close(sock);
    this->is_binded = false;
  }

  //---------------------------
}

//Subfunctions
int Socket_client::compute_recv_code(vector<int> packet_dec){
  int code = 0;
  //---------------------------

  if(packet_dec.size() == 3){
    if(packet_dec[0] == 50 && packet_dec[1] == 48 && packet_dec[2] == 48){
      code = 200;
    }
  }

  //---------------------------
  return code;
}
