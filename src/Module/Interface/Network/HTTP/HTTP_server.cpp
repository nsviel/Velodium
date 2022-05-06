#include "HTTP_server.h"
//http://localhost:8888/

#include "../../Interface_node.h"
#include "../../Local/Saving.h"

#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#define PATH_IMAGE "picture.png"


//Constructor / Destructor
HTTP_server::HTTP_server(Interface_node* node){
  //---------------------------

  Saving* saveManager = node->get_saveManager();

  this->path_image = saveManager->get_path_image() + "image";
  this->server_port = 8888;

  //---------------------------
  this->start_deamon();
}
HTTP_server::~HTTP_server(){}

// ON / OFF http daemon
void HTTP_server::start_deamon(){
  //---------------------------

  const char* page = "<html><head><title>Test HTTP server</title></head></html>";
  page = path_image.c_str();

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
void HTTP_server::loop(){
  //---------------------------




  //---------------------------
  return;
}

//Daemon functions
int HTTP_server::print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
  //---------------------------

  printf ("%s: %s\n", key, value);

  //---------------------------
  return MHD_YES;
}
void HTTP_server::print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version){
  //---------------------------

  cout<<"---------------------"<<endl;
  printf ("New %s method for url %s using version %s\n", method, url, version);
  cout<<"-------"<<endl;
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);
  cout<<"---------------------"<<endl;

  //---------------------------
}
int HTTP_server::answer_to_connection(void *cls, struct MHD_Connection *connection, const char *url,
  const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **history){
  int ret;
  const char *me = (char*) cls;
  string path(me);
  //---------------------------
  say(path);

  //Print connection info
  //print_info(connection, url, method, version);

  //Check input method
  if(strcmp(method, "GET") == 0){

    //Open file
    int fd = open (path.c_str(), O_RDONLY);
    struct stat sbuf;
    int fs = fstat (fd, &sbuf);

    //Check file opening
    if(fd == -1 || fs != 0){
      close (fd);
      const char *errorstr = "<html><body>An internal server error has occurred!</body></html>";
      struct MHD_Response* response = MHD_create_response_from_buffer (strlen (errorstr), (void *) errorstr, MHD_RESPMEM_PERSISTENT);
      if(response){
        int ret = MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
        MHD_destroy_response (response);
        return MHD_YES;
      }
      else{
        return MHD_NO;
      }
    }

    //Send file
    struct MHD_Response* response = MHD_create_response_from_fd64(sbuf.st_size, fd);
    if (NULL == response){if (0 != close (fd))abort ();return MHD_NO;}
    MHD_add_response_header(response, "Content-Type", "image/bmp");
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

  }else{
    return MHD_NO;
  }

  //---------------------------
  return ret;
}
