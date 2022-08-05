#ifndef HTTPS_DAEMON_H
#define HTTPS_DAEMON_H

#include "../../../../common.h"

#include <microhttpd.h>

class Command;


class Daemon
{
public:
  Daemon(Command* command);
  ~Daemon();

public:
  //Main daemon functions
  void set_configuration(int port);
  void start_deamon();
  void stop_deamon();

  //Daemon functions
  static int http_answer(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls);
  static int http_send_ok(void* cls, struct MHD_Connection* connection);
  static int http_send_error(void* cls, struct MHD_Connection* connection);
  static int http_send_image(void *cls, struct MHD_Connection *connection);

  //Get request functions
  static void http_get_slam_on();
  static void http_get_slam_off();
  static void http_get_view_top();
  static void http_get_view_oblique();

  //Accesseur
  inline void set_path_image(string value){this->path_image = value;}
  inline bool get_is_daemon(){return is_deamon;}

private:
  struct MHD_Daemon* daemon;

  Command* commandManager;

  string path_image;
  bool is_deamon;
  bool is_first_get;
  int server_port;

  static bool slam;
};

#endif
