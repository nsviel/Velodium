#ifndef UDP_SERVER_H
#define UDP_SERVER_H

// Server side implementation of UDP client-server model

#include "../../common.h"

class UDP_server
{
public:
  //Constructor / Destructor
  UDP_server();
  ~UDP_server();

public:
  //Main functions
  vector<int> read_UDP_packets(int port);

  //Subfunctions
  void server_binding(int port);
  void server_unbinding();
  void server_read_data();

  inline bool* get_is_binded(){return &is_binded;}

private:
  vector<int> packet_dec;
  bool is_binded;
  int packet_size;
  int sock;
};

#endif
