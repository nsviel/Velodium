#include "http_daemon.h"

#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#include <fstream>


//Constructor / Destructor
http_daemon::http_daemon(){
  //---------------------------

  //---------------------------
}
http_daemon::~http_daemon(){}

// ON / OFF http daemon
void http_daemon::set_configuration(int port){
  //---------------------------

  this->server_port = port;
  this->is_first_get = true;
  this->is_deamon = false;

  //---------------------------
}
void http_daemon::start_deamon(){
  //---------------------------

  const char* page = path_image.c_str();
  this->daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, server_port, NULL, NULL, http_answer, (void*)page, MHD_OPTION_END);

  if(daemon == NULL){
    console.AddLog("error", "Problem with HTTP server");
    this->is_deamon = false;
  }else{
    string log = "HTTP server online on " + to_string(server_port);
    console.AddLog("ok", log);
    this->is_deamon = true;
  }

  //---------------------------
}
void http_daemon::stop_deamon(){
  //---------------------------

  MHD_stop_daemon(daemon);

  //---------------------------
}

//http_daemon functions
enum MHD_Result http_daemon::http_answer(void* cls, struct MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void* *history){
  //---------------------------

  //Check input method
  enum MHD_Result ret;
  //MHD_Result response;
  if(strcmp(method, "GET") == 0){
    if(strcmp(url, "/test_http_conn") == 0){
      ret = http_send_ok(cls, connection);
    }
    else if(strcmp(url, "/image") == 0){
      ret = http_send_image(cls, connection);
    }
    else if(strcmp(url, "/slam_on") == 0){
      http_get_slam_on();
      ret = http_send_ok(cls, connection);
    }
    else if(strcmp(url, "/slam_off") == 0){
      http_get_slam_off();
      ret = http_send_ok(cls, connection);
    }
    else if(strcmp(url, "/view_top") == 0){
      http_get_view_top();
      ret = http_send_ok(cls, connection);
    }
    else if(strcmp(url, "/view_oblique") == 0){
      http_get_view_oblique();
      ret = http_send_ok(cls, connection);
    }
  }

  //---------------------------
  return ret;
}
enum MHD_Result http_daemon::http_send_ok(void* cls, struct MHD_Connection* connection){
  //---------------------------

  const char* errorstr = "<html><body>ok</body></html>";
  struct MHD_Response* response = MHD_create_response_from_buffer (strlen (errorstr), (void* ) errorstr, MHD_RESPMEM_PERSISTENT);
  if(response){
    enum MHD_Result ret = MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
    MHD_destroy_response (response);
    return MHD_YES;
  }
  else{
    return MHD_NO;
  }

  //---------------------------
}
enum MHD_Result http_daemon::http_send_error(void* cls, struct MHD_Connection* connection){
  //---------------------------

  const char* errorstr = "<html><body>An internal server error has occurred!</body></html>";
  struct MHD_Response* response = MHD_create_response_from_buffer (strlen (errorstr), (void* ) errorstr, MHD_RESPMEM_PERSISTENT);
  if(response){
    enum MHD_Result ret = MHD_queue_response (connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
    MHD_destroy_response (response);
    return MHD_YES;
  }
  else{
    return MHD_NO;
  }

  //---------------------------
}
enum MHD_Result http_daemon::http_send_image(void* cls, struct MHD_Connection* connection){
  //---------------------------

  //Get file path
  const char* me = (char*) cls;
  string path(me);

  //Open file
  int fd = open (path.c_str(), O_RDONLY);
  struct stat sbuf;
  int fs = fstat (fd, &sbuf);

  //Check file opening
  if(fd == -1 || fs != 0){
    close (fd);
    return http_send_error(cls, connection);
  }

  //Send file
  struct MHD_Response* response = MHD_create_response_from_fd64(sbuf.st_size, fd);
  if (NULL == response){if (0 != close (fd))abort ();return MHD_NO;}
  MHD_add_response_header(response, "Content-Type", "image/bmp");
  enum MHD_Result ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
  MHD_destroy_response(response);

  //---------------------------
  return ret;
}

// GET request handlers
void http_daemon::http_get_slam_on(){
  //---------------------------

  ofstream file;
  file.open("../media/engine/config/http_conf.txt", std::ios_base::app);
  file << "slam true\n";
  file.close();

  //---------------------------
}
void http_daemon::http_get_slam_off(){
  //---------------------------

  ofstream file;
  file.open ("../media/engine/config/http_conf.txt", std::ios_base::app);
  file << "slam false\n";
  file.close();

  //---------------------------
}
void http_daemon::http_get_view_top(){
  //---------------------------

  ofstream file;
  file.open ("../media/engine/config/http_conf.txt", std::ios_base::app);
  file << "view top\n";
  file.close();

  //---------------------------
}
void http_daemon::http_get_view_oblique(){
  //---------------------------

  ofstream file;
  file.open ("../media/engine/config/http_conf.txt", std::ios_base::app);
  file << "view oblique\n";
  file.close();

  //---------------------------
}
