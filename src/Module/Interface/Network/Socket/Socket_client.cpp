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
  this->port_src = 9999;

  //---------------------------
}
Socket_client::~Socket_client(){}

//Socket function
void Socket_client::socket_binding(int port, string ip){
  if(is_binded == false){
    this->port_dest = port;
    this->ip_dest = ip;
    //---------------------------

    // Creating socket file descriptor
    this->sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sock < 0){
      cout << "socket creation failed" << endl;
      exit(EXIT_FAILURE);
    }

    // Filling server information
    sockaddr_in addr;
  	addr.sin_port = htons(port_src);
  	addr.sin_family = AF_INET;

    // Bind the socket with the server address
    int binding = bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));

    //Check final success
    if(binding == 0){
      this->is_binded = true;
    }else{
      cout << "[error] Client socket binding failed for port [" << port_dest << "]" << endl;
      this->is_binded = false;
    }

    //---------------------------
  }
}
void Socket_client::socket_send_data(string data){
  //---------------------------

  //Parameter
  sockaddr_in dst = { 0 };
	inet_pton(AF_INET, ip_dest.c_str(), &dst.sin_addr.s_addr);
	dst.sin_family = AF_INET;
	dst.sin_port = htons(port_dest);

  //Send data
  int ret = sendto(sock, data.data(), static_cast<int>(data.length()), 0, reinterpret_cast<const sockaddr*>(&dst), sizeof(dst));

  //Check final success
  if (ret < 0){
    cout << "[error] Client socket data sending failed for port [" << port_dest << "]" << endl;
  }

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
