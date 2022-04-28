#ifndef HTTP_BOOST_H
#define HTTP_BOOST_H

#include "../../../../common.h"



class HTTP_server
{
public:
  HTTP_server();
  ~HTTP_server();

public:
  void start_deamon();
  void stop_deamon();
  void loop();

private:
  struct MHD_Daemon* daemon;
};

#endif
