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
  void update_configuration();
  
  // ON / OFF http daemon
  void start_deamon();
  void stop_deamon();

  //Daemon functions
  static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
  static void print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version);
  static int http_post_geolocalization(void *cls, struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size);
  static int http_get_image(void *cls, struct MHD_Connection *connection);
  static int http_answer(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);

  inline bool get_is_daemon(){return is_deamon;}

private:
  Saving* saveManager;

  struct MHD_Daemon* daemon;

  string path_image;
  bool with_http_demon;
  bool is_deamon;
  int server_port;
};

#endif
