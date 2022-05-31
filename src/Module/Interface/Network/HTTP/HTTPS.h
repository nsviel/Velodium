#ifndef PROTOCOL_HTTPS_H
#define PROTOCOL_HTTPS_H

#include "../../../../common.h"

#include <microhttpd.h>

class Network;
class Saving;
class Configuration;


class HTTPS
{
public:
  HTTPS(Network* netManager);
  ~HTTPS();

public:
  void update_configuration();

  // ON / OFF http daemon
  void start_deamon();
  void stop_deamon();

  //Daemon functions
  static int http_answer(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);
  static int http_post_geolocalization(void *cls, struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size);
  static int http_get_image(void *cls, struct MHD_Connection *connection);

  //Subfunctions
  static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
  static void print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version);

  inline bool get_is_daemon(){return is_deamon;}

private:
  Saving* saveManager;
  Configuration* configManager;

  struct MHD_Daemon* daemon;

  string path_image;
  bool with_http_demon;
  bool is_deamon;
  int server_port;
};

#endif
