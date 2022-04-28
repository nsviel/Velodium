#include "HTTP_server.h"


#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#define PORT 8888

//Constructor / Destructor
HTTP_server::HTTP_server(){
  //---------------------------

  this->start_deamon();

  //---------------------------
}
HTTP_server::~HTTP_server(){}

int answer_to_connection (void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **con_cls){
  //---------------------------

  const char *page = "<html><body>Hello, browser!</body></html>";

  struct MHD_Response* response = MHD_create_response_from_buffer (strlen (page), (void*) page, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  //---------------------------
  return ret;
}
void HTTP_server::start_deamon(){
  //---------------------------

  this->daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL, &answer_to_connection, NULL, MHD_OPTION_END);
  if (NULL == daemon){
    return;
  }

  //---------------------------
}
void HTTP_server::stop_deamon(){
  //---------------------------

  MHD_stop_daemon (daemon);

  //---------------------------
}



void HTTP_server::loop(){
  //---------------------------




  //---------------------------
  return;
}
