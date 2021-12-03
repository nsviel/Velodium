#include "Sender.h"

#include "../../Specific/fct_maths.h"

#include <filesystem>
#include <arpa/inet.h>


//Constructor / Destructor
Sender::Sender(){
  //---------------------------

  //---------------------------
}
Sender::~Sender(){}

void Sender::send_something(){
  //---------------------------

  FILE *filehandle = fopen("imagefile.jpg", "rb");
  if (filehandle != NULL){
    int clientsocket = 0;
    sendfile(clientsocket, filehandle);
    fclose(filehandle);
  }

  //---------------------------
}

bool Sender::senddata(int sock, void *buf, int buflen){
  unsigned char *pbuf = (unsigned char *) buf;
  //---------------------------

  while (buflen > 0){
    int num = send(sock, pbuf, buflen, 0);
    if (num == SO_ERROR){
      return false;
    }

    pbuf += num;
    buflen -= num;
  }

  //---------------------------
  return true;
}
bool Sender::sendfile(int sock, FILE *f){
  //---------------------------

  fseek(f, 0, SEEK_END);
  long filesize = ftell(f);
  rewind(f);

  //Check
  if (filesize == EOF){
    return false;
  }

  filesize = htonl(filesize);
  bool sucess = senddata(sock, &filesize, sizeof(filesize));

  //Check
  if (!sucess){
    return false;
  }

  /*if (filesize > 0){
    char buffer[1024];
    do{
      size_t num = filesize; //fct_min(filesize, sizeof(buffer));
      num = fread(buffer, 1, num, f);

      if (num < 1){
        return false;
      }
      if (!senddata(sock, buffer, num, 0)){
        return false;
      }

      filesize -= num;
    }
    while (filesize > 0);
  }*/

  //---------------------------
  return true;
}
