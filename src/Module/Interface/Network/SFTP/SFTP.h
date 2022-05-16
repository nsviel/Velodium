#ifndef PROTOCOL_SFTP_H
#define PROTOCOL_SFTP_H

#include "../../../../common.h"

#include <libssh/sftp.h>
#include <libssh/libssh.h>
#include <fcntl.h>
#include <fstream>

class Network;
class SSH;


class SFTP
{
public:
  //Constructor / Destructor
  SFTP(Network* netManager);
  ~SFTP();

public:
  void sftp_new_session();
  void sftp_disconnection();

  void sftp_send_file(string path_src, string path_trg, string file_name);
  void sftp_create_directory(string dirName);

private:
  SSH* sshManager;

  sftp_session sftp;
  bool sftp_connected;
};

#endif
