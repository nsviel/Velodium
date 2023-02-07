#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

// Server side implementation of UDP client-server model

#include "../../../common.h"

class Socket_client
{
public:
  //Constructor / Destructor
  Socket_client();
  ~Socket_client();

public:
  //Socket functions
  void socket_binding(int port, string ip);
  void socket_send_data(string data);
  void socket_disconnect();

  //Subfunctions
  int compute_recv_code(vector<int> packet_dec);

  inline vector<int> get_data_dec(){return packet_dec;}
  inline bool get_is_binded(){return is_binded;}
  inline int get_status_code(){return code;}

private:
  vector<int> packet_dec;
  bool is_binded;
  string ip_dest;
  int port_src;
  int port_dest;
  int sock;
  int code;
};

#endif
