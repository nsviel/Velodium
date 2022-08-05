#ifndef PROTOCOL_HTTPS_H
#define PROTOCOL_HTTPS_H

#include "../../../../common.h"

class Configuration;
class Network;
class Saving;
class Daemon;
class Command;


class HTTPS
{
public:
  HTTPS(Network* netManager);
  ~HTTPS();

public:
  void update_configuration();
  void start_server();
  void stop_server();

  inline bool get_is_https_deamon(){return is_https_deamon;}
  inline int get_server_port(){return server_port;}

private:
  Configuration* configManager;
  Daemon* daemonManager;
  Command* commandManager;
  Saving* saveManager;

  bool with_http_demon;
  bool is_https_deamon;
  string path_image;
  int server_port;
};

#endif
