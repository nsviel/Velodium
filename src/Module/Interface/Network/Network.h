#ifndef NETWORK_H
#define NETWORK_H

#include "../../../common.h"

#include <libssh/libssh.h>

class MQTT;
class SFTP;
class SSH;


class Network
{
public:
  //Constructor / Destructor
  Network();
  ~Network();

public:
  //Main function
  void send_file(string path_source, string path_target);
  void start_connection();
  void stop_connection();

  //Threaded functions
  void start_thread_mqtt();
  void start_thread_image();

  //Subfunctions
  void select_sourcePath();
  void select_targetPath();

  inline MQTT* get_mqttManager(){return mqttManager;}
  inline SFTP* get_sftpManager(){return sftpManager;}
  inline SSH* get_sshManager(){return sshManager;}
  inline string* get_path_source(){return &path_source;}
  inline string* get_path_target(){return &path_target;}
  inline bool get_is_connected(){return is_connected;}
  inline bool get_is_mqtt_watcher(){return is_mqtt_watcher;}
  inline bool get_is_image_watcher(){return is_image_watcher;}

private:
  MQTT* mqttManager;
  SFTP* sftpManager;
  SSH* sshManager;

  string name_file;
  string path_source;
  string path_target;

  bool is_mqtt_watcher;
  bool is_image_watcher;
  bool is_connected;
};

#endif
