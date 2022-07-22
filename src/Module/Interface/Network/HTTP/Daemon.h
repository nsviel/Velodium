#ifndef HTTPS_DAEMON_H
#define HTTPS_DAEMON_H

#include "../../../../common.h"

#include <microhttpd.h>


class Daemon
{
public:
  Daemon();
  ~Daemon();

public:
  //Main daemon functions
  void set_configuration(int port);
  void start_deamon();
  void stop_deamon();

  //Daemon functions
  static int http_answer(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);
  static int http_post_cbor(struct MHD_Connection* connection, const char* upload_data, size_t* upload_data_size);
  static int http_post_geo(struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size);
  static int http_get_image(void *cls, struct MHD_Connection *connection);
  static int http_get_no_slam(void* cls, struct MHD_Connection* connection);
  static int http_get_with_slam(void* cls, struct MHD_Connection* connection);

  //Subfunctions
  static int send_page (struct MHD_Connection *connection, const char* page, int status_code);
  static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
  static void print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version);

  //Accesseur
  inline void set_path_image(string value){this->path_image = value;}
  inline bool get_is_daemon(){return is_deamon;}

private:
  struct MHD_Daemon* daemon;

  string path_image;
  bool is_deamon;
  bool is_first_get;
  int server_port;
};

#endif
