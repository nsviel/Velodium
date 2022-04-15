#ifndef PROTOCOL_SFTP_H
#define PROTOCOL_SFTP_H

#include "../../../../common.h"

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
  void sftp_send_file(ssh_session& ssh, string path_src, string path_trg);
  void sftp_create_directory(ssh_session& ssh, string dirName);

  void sftp_new_session();
  void sftp_disconnection();

private:
  sftp_session sftp;
  bool sftp_connected;
};

#endif
