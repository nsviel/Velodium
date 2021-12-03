#ifndef SENDER_H
#define SENDER_H

#include "../../common.h"

#include <sys/socket.h>


class Sender
{
public:
  //Constructor / Destructor
  Sender();
  ~Sender();

public:
  void send_something();

private:
  bool senddata(int sock, void *buf, int buflen);
  bool sendfile(int sock, FILE *f);

private:

};

#endif
