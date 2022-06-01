#include "HTTPS.h"
//http://localhost:8888/

#include "../Network.h"
#include "../../Interface_node.h"
#include "../../Local/Saving.h"

#include "../../../../Engine/Engine_node.h"
#include "../../../../Engine/Scene/Configuration.h"

#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#define PATH_IMAGE "picture.png"
bool FIRST_GET=true;


//Constructor / Destructor
HTTPS::HTTPS(Network* netManager){
  //---------------------------

  Interface_node* node_interface = netManager->get_node_interface();
  Engine_node* node_engine = node_interface->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->saveManager = node_interface->get_saveManager();

  this->is_deamon = false;

  //---------------------------
  this->update_configuration();
}
HTTPS::~HTTPS(){}

void HTTPS::update_configuration(){
  //---------------------------

  this->path_image = saveManager->get_path_image() + "image";
  this->with_http_demon = configManager->parse_json_b("network", "with_http_demon");
  this->server_port = configManager->parse_json_i("network", "http_port");
  this->is_first_get = true;

  //---------------------------
  this->start_deamon();
}

// ON / OFF http daemon
void HTTPS::start_deamon(){
  if(with_http_demon){
    //---------------------------

    const char* page = path_image.c_str();
    this->daemon = MHD_start_daemon (MHD_USE_INTERNAL_POLLING_THREAD, server_port, NULL, NULL, http_answer, (void*)page, MHD_OPTION_END);

    if(daemon == NULL){
      cout<<"[error] Problem with HTTP server"<<endl;
      this->is_deamon = false;
    }else{
      this->is_deamon = true;
    }

    //---------------------------
  }
}
void HTTPS::stop_deamon(){
  //---------------------------

  MHD_stop_daemon(daemon);

  //---------------------------
}

//Daemon functions
int HTTPS::http_answer(void *cls, struct MHD_Connection *connection, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **history){
  //---------------------------

  //Check input method
  int ret;
  if(strcmp(method, "GET") == 0){
    ret = http_get_image(cls, connection);
  }
  if(strcmp(method, "POST") == 0){
    ret = http_post_geolocalization(cls, connection, upload_data, upload_data_size, history);
  }

  //---------------------------
  return ret;
}
int HTTPS::http_get_image(void *cls, struct MHD_Connection *connection){
  //---------------------------

  //Get file path
  const char *me = (char*) cls;
  string path(me);

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
  int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);

  //---------------------------
  return ret;
}
int HTTPS::http_post_geolocalization(void* cls, struct MHD_Connection *connection, const char *upload_data, size_t *upload_data_size, void **con_cls){
  struct connection_info_struct *con_info = new connection_info_struct();
  int ret;
  //---------------------------

  if(FIRST_GET){
    FIRST_GET = false;
    return MHD_YES;
  }

  if(*upload_data_size != 0){
    *upload_data_size = 0;

    //Retrieve data
    string data(upload_data);
    say(data); //----> ici, les données trasmises !

    //Return a response to the client
    const char* page = "[ok] Data received\n";
    ret = send_page(connection, page, 200);
  }
  else{
    const char* page = "[error] No data received by the HTTP server\n";
    ret = send_page(connection, page, 200);
  }

  //---------------------------
  FIRST_GET = true;
  return MHD_YES;
}

//Subfunctions
int HTTPS::send_page (struct MHD_Connection *connection, const char* page, int status_code){
  //---------------------------

  struct MHD_Response* response = MHD_create_response_from_buffer (strlen (page), (void*) page, MHD_RESPMEM_MUST_COPY);
  if (!response) return MHD_NO;
  MHD_add_response_header(response, "Content-Type", "text/plain");
  int ret = MHD_queue_response (connection, status_code, response);
  MHD_destroy_response (response);

  //---------------------------
  return ret;
}
int HTTPS::print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value){
  //---------------------------

  printf ("%s: %s\n", key, value);

  //---------------------------
  return MHD_YES;
}
void HTTPS::print_info(struct MHD_Connection *connection, const char *url, const char *method, const char *version){
  //---------------------------

  cout<<"---------------------"<<endl;
  printf ("New %s method for url %s using version %s\n", method, url, version);
  cout<<"-------"<<endl;
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);
  cout<<"---------------------"<<endl;

  //---------------------------
}
