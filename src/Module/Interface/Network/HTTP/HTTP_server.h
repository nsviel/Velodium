#ifndef HTTP_BOOST_H
#define HTTP_BOOST_H

#include "../../../../common.h"

#include <microhttpd.h>

class Interface_node;
class Saving;


class HTTP_server
{
public:
  HTTP_server(Interface_node* node);
  ~HTTP_server();

public:
  // ON / OFF http daemon
  void start_deamon();
  void stop_deamon();
  void loop();

  //Daemon functions
  static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
  static void print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version);
  static int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);

private:
  Saving* saveManager;

  struct MHD_Daemon* daemon;

  string path_image;
  int server_port;
};

#endif
