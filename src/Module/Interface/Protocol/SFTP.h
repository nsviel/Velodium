#ifndef PROTOCOL_SFTP_H
#define PROTOCOL_SFTP_H

#include "../../../common.h"

#include <libssh/sftp.h>
#include <libssh/libssh.h>
#include <fcntl.h>
#include <fstream>


class SFTP
{
public:
  //Constructor / Destructor
  SFTP();
  ~SFTP();

public:
  void sftp_sendFile(ssh_session& ssh, string path_src, string path_trg);
  void sftp_createDirectory(ssh_session& ssh, string dirName);

private:
  sftp_session sftp;
};

#endif
