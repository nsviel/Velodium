#include "HTTP_server.h"
//http://localhost:8888/

#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>


//Constructor / Destructor
HTTP_server::HTTP_server(){
  //---------------------------

  this->server_port = 8888;

  //---------------------------
  this->start_deamon();
}
HTTP_server::~HTTP_server(){}

// ON / OFF http daemon
void HTTP_server::start_deamon(){
  //---------------------------

  const char* page = "<html><head><title>libmicrohttpd demo</title></head><body>libmicrohttpd demo</body></html>";

  this->daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, server_port,
                                   NULL, NULL, answer_to_connection, (void*)page, MHD_OPTION_END);
  if(daemon == NULL){
    return;
  }

  //---------------------------
}
void HTTP_server::stop_deamon(){
  //---------------------------

  MHD_stop_daemon(daemon);

  //---------------------------
}

int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
  printf ("%s: %s\n", key, value);
  return MHD_YES;
}

//Subfunctions
int HTTP_server::answer_to_connection(void *cls, struct MHD_Connection *connection, const char *url,
  const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **history){
  //---------------------------

  printf ("New [method] %s method for [url] %s using [version] %s\n", method, url, version);
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);

  // unexpected method
  if(strcmp(method, "GET") != 0){
    return MHD_NO;
  }

  // do never respond on first call
  static int aptr;
  if(*history != &aptr){
    *history = &aptr;
    return MHD_YES;
  }
  *history = NULL;

  const char *page = (char*)cls;
  struct MHD_Response* response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT);
  int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);

  //---------------------------
  return ret;
}
void HTTP_server::loop(){
  //---------------------------




  //---------------------------
  return;
}
