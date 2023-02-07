#ifndef CAPTURE_SERVER_H
#define CAPTURE_SERVER_H

// Server side implementation of UDP client-server model

#include "../../../common.h"

class Socket_server;
class Socket_client;


class Capture_server
{
public:
  //Constructor / Destructor
  Capture_server();
  ~Capture_server();

public:
  //Main functions
  void capture_init(int port, int packet_size);
  void capture_stop();
  vector<int> capture_packet();

private:
  Socket_server* sock_server;
  Socket_client* sock_client;
};

#endif
