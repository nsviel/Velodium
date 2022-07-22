#include "Daemon.h"
//http://localhost:8888/
//Documentation: https://www.gnu.org/software/libmicrohttpd/manual/libmicrohttpd.html#toc-Introduction
//Tutorial: https://www.gnu.org/software/libmicrohttpd/tutorial.html

#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */

#define PATH_IMAGE "picture.png"
int cpt=0;


//Constructor / Destructor
Daemon::Daemon(){
  //---------------------------

  //---------------------------
}
Daemon::~Daemon(){}

// ON / OFF http daemon
void Daemon::set_configuration(int port){
  //---------------------------

  this->server_port = port;
  this->is_first_get = true;
  this->is_deamon = false;

  //---------------------------
}
void Daemon::start_deamon(){
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
void Daemon::stop_deamon(){
  //---------------------------

  MHD_stop_daemon(daemon);

  //---------------------------
}

//Daemon functions
int Daemon::http_answer(void* cls, struct MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void* *history){
  //---------------------------

  //Check input method
  int ret;
  if(strcmp(method, "GET") == 0){
    if(strcmp(url, "/image") == 0){
      ret = http_get_image(cls, connection);
    }
    if(strcmp(url, "/no_slam") == 0){
      ret = http_get_no_slam(cls, connection);
    }
    if(strcmp(url, "/with_slam") == 0){
      ret = http_get_with_slam(cls, connection);
    }
  }
  if(strcmp(method, "POST") == 0){
    if(strcmp(url, "/cbor") == 0){
      ret = http_post_cbor(connection, upload_data, upload_data_size);
    }
    if(strcmp(url, "/geo") == 0){
      ret = http_post_geo(connection, upload_data, upload_data_size);
    }
  }

  //---------------------------
  return ret;
}
int Daemon::http_get_image(void* cls, struct MHD_Connection* connection){
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
    const char* errorstr = "<html><body>An internal server error has occurred!</body></html>";
    struct MHD_Response* response = MHD_create_response_from_buffer (strlen (errorstr), (void* ) errorstr, MHD_RESPMEM_PERSISTENT);
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
int Daemon::http_get_no_slam(void* cls, struct MHD_Connection* connection){
  //---------------------------

  say("no slam http command");

  //---------------------------
  return MHD_YES;
}
int Daemon::http_get_with_slam(void* cls, struct MHD_Connection* connection){
  //---------------------------

  say("with slam http command");

  //---------------------------
  return MHD_YES;
}

http_get_with_slam
int Daemon::http_post_cbor(struct MHD_Connection* connection, const char* upload_data, size_t* upload_data_size){
  static bool is_received = false;
  //---------------------------

  //I dont know why but a 1 & 3 call is received empty (?)
  if(cpt == 0){
    cpt++;
    return MHD_YES;
  }

  //Process received data
  if(cpt == 1 && *upload_data_size != 0){
    *upload_data_size = 0;

    //Retrieve data
    string data(upload_data);
    say(data); //----> ici, les données transmises !

    cpt++; is_received = true;
    return MHD_YES;
  }else if(cpt == 1){
    cpt++; is_received = false;
    return MHD_YES;
  }

  //Send post response
  int ret;
  if(cpt == 2 && is_received == true){
    const char* page = "[ok] Data received\n";
    ret = send_page(connection, page, 200);
    cpt = 0; is_received = false;
  }else if(cpt == 2 && is_received == false){
    const char* page = "[error] No data received by the HTTP server\n";
    ret = send_page(connection, page, 200);
    cpt = 0; is_received = false;
  }

  //---------------------------
  return ret;
}
int Daemon::http_post_geo(struct MHD_Connection* connection, const char* upload_data, size_t* upload_data_size){
  static bool is_received = false;
  //---------------------------

  //I dont know why but a 1 & 3 call is received empty (?)
  if(cpt == 0){
    cpt++;
    return MHD_YES;
  }

  //Process received data
  if(cpt == 1 && *upload_data_size != 0){
    *upload_data_size = 0;

    //Retrieve data
    string data(upload_data);
    say(data); //----> ici, les données transmises !

    cpt++; is_received = true;
    return MHD_YES;
  }else if(cpt == 1){
    cpt++; is_received = false;
    return MHD_YES;
  }

  //Send post response
  int ret;
  if(cpt == 2 && is_received == true){
    const char* page = "[ok] Data received\n";
    ret = send_page(connection, page, 200);
    cpt = 0; is_received = false;
  }else if(cpt == 2 && is_received == false){
    const char* page = "[error] No data received by the HTTP server\n";
    ret = send_page(connection, page, 200);
    cpt = 0; is_received = false;
  }

  //---------------------------
  return ret;
}

//Subfunctions
int Daemon::send_page (struct MHD_Connection* connection, const char* page, int status_code){
  //---------------------------

  struct MHD_Response* response = MHD_create_response_from_buffer (strlen (page), (void*) page, MHD_RESPMEM_MUST_COPY);
  if (!response) return MHD_NO;
  MHD_add_response_header(response, "Content-Type", "text/plain");
  int ret = MHD_queue_response (connection, status_code, response);
  MHD_destroy_response (response);

  //---------------------------
  return ret;
}
int Daemon::print_out_key (void* cls, enum MHD_ValueKind kind, const char* key, const char* value){
  //---------------------------

  printf ("%s: %s\n", key, value);

  //---------------------------
  return MHD_YES;
}
void Daemon::print_info(struct MHD_Connection* connection, const char* url, const char* method, const char* version){
  //---------------------------

  cout<<"---------------------"<<endl;
  printf ("New %s method for url %s using version %s\n", method, url, version);
  cout<<"-------"<<endl;
  MHD_get_connection_values (connection, MHD_HEADER_KIND, &print_out_key, NULL);
  cout<<"---------------------"<<endl;

  //---------------------------
}
