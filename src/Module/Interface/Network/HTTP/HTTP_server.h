#ifndef HTTP_BOOST_H
#define HTTP_BOOST_H

#include "../../../../common.h"



class HTTP_server
{
public:
  HTTP_server();
  ~HTTP_server();

public:
  // ON / OFF http daemon
  void start_deamon();
  void stop_deamon();

  //Subfunctions
  void loop();
  static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);

private:
  struct MHD_Daemon* daemon;
  int server_port;
};

#endif
