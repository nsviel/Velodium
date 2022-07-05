#ifndef SOCKET_H
#define SOCKET_H

// Server side implementation of UDP client-server model

#include "../../../../common.h"

class Socket
{
public:
  //Constructor / Destructor
  Socket();
  ~Socket();

public:
  //Socket functions
  void socket_binding(int port, int packet_size);
  void socket_recv_data();
  void socket_send_data();
  void socket_disconnect();

  //Subfunctions
  int compute_recv_code(vector<int> packet_dec);

  inline vector<int> get_data_dec(){return packet_dec;}
  inline bool get_is_binded(){return is_binded;}
  inline void set_port(int value){this->port = value;}

private:
  vector<int> packet_dec;
  bool is_binded;
  int packet_size;
  int port;
  int sock;
  int code;
};

#endif
