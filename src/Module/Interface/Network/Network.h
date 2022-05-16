#ifndef NETWORK_H
#define NETWORK_H

#include "struct_wallet.h"
#include "../../../common.h"

#include <libssh/libssh.h>

class Interface_node;
class Configuration;
class HTTP_server;
class MQTT;
class SFTP;
class SSH;


class Network
{
public:
  //Constructor / Destructor
  Network(Interface_node* node);
  ~Network();

public:
  //Main function
  void update_configuration();
  void create_wallet();
  string get_ip_from_dest(string dest);

  //Connection function
  void send_file();
  void start_connection();
  void stop_connection();
  void select_sourcePath();
  void select_targetPath();

  inline MQTT* get_mqttManager(){return mqttManager;}
  inline SFTP* get_sftpManager(){return sftpManager;}
  inline SSH* get_sshManager(){return sshManager;}
  inline HTTP_server* get_httpManager(){return httpManager;}
  inline Wallet* get_wallet(){return wallet;}
  inline Interface_node* get_node_interface(){return node_interface;}

  inline string* get_path_source(){return &path_source;}
  inline string* get_path_target(){return &path_target;}
  inline bool get_is_connected(){return is_connected;}
  inline bool get_is_mqtt_watcher(){return is_mqtt_watcher;}
  inline bool get_is_image_watcher(){return is_image_watcher;}

private:
  Configuration* configManager;
  Interface_node* node_interface;
  MQTT* mqttManager;
  SFTP* sftpManager;
  SSH* sshManager;
  HTTP_server* httpManager;
  Wallet* wallet;

  string name_file;
  string path_source;
  string path_target;

  bool is_mqtt_watcher;
  bool is_image_watcher;
  bool is_connected;
};

#endif
