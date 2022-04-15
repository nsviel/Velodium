#include "SSH.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / Destructor
SSH::SSH(){
  //---------------------------

  this->selected_port = 22;
  this->ssh_connected = false;
  this->selected_dest = "localhost";
  this->selected_ip = "127.0.0.1";

  //---------------------------
}
SSH::~SSH(){}

void SSH::ssh_connection(){
  //---------------------------

  if(ssh_connected == false){
    // Run new SSH session
    this->ssh_new_session();

    // Verify the server's identity
    this->ssh_autentification_server();

    // Authenticate ourselves
    //this->ssh_autentification_user();

    //Set connection flag on
    ssh_connected = true;
  }

  //---------------------------
}
void SSH::ssh_disconnection(){
  //---------------------------

  if(ssh_connected){
    ssh_disconnect(ssh);
    ssh_free(ssh);
    this->ssh_connected = false;
  }

  //---------------------------
}

//Subfunctions
void SSH::ssh_new_session(){
  //---------------------------

  //SSH allocation
  this->ssh = ssh_new();
  if(ssh == NULL){
    cout<<"Error SSH connection"<<endl;
    return;
  }

  //Options
  ssh_options_set(ssh, SSH_OPTIONS_HOST, selected_ip.c_str());
  ssh_options_set(ssh, SSH_OPTIONS_PORT, &selected_port);

  int rc = ssh_connect(ssh);
  if (rc != SSH_OK){
    cout<<"Error connecting to "<<selected_ip<<":"<<selected_port<<" ["<<ssh_get_error(ssh)<<"]"<<endl;
    return;
  }

  //---------------------------
}
void SSH::ssh_autentification_server(){
  //SSH host file: (~/.ssh/known_hosts on UNIX)
  //---------------------------

  bool success = true;
  unsigned char *hash = NULL;
  ssh_key srv_pubkey = NULL;
  size_t hlen;
  int rc;

  //Public key stuff
  rc = ssh_get_server_publickey(ssh, &srv_pubkey);
  if (rc < 0) {return;}
  rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  if (rc < 0) {return;}
  ssh_key_free(srv_pubkey);

  //Check is the server is known
  enum ssh_known_hosts_e state = ssh_session_is_known_server(ssh);
  switch (state) {
    case SSH_KNOWN_HOSTS_OK:{
      cout<<"Server is known."<<endl;
      break;
    }
    case SSH_KNOWN_HOSTS_CHANGED:{
      cout<< "Host key for server changed"<<endl;
      cout<< "For security reasons, connection will be stopped"<<endl;
      ssh_clean_pubkey_hash(&hash);
      return;
    }
    case SSH_KNOWN_HOSTS_OTHER:{
      cout<< "The host key for this server was not found but an other type of key exists."<<endl;
      cout<< "An attacker might change the default server key to confuse your client into thinking the key does not exist"<<endl;
      ssh_clean_pubkey_hash(&hash);
      return;
    }
    case SSH_KNOWN_HOSTS_NOT_FOUND:{
      cout<< "Could not find known host file."<<endl;
      cout<< "If you accept the host key here, the file will be automatically created."<<endl;
    }
    case SSH_KNOWN_HOSTS_UNKNOWN:{
      //Print new public key
      char* hexa = ssh_get_hexa(hash, hlen);
      cout<<"The server is unknown, host key :"<<endl;
      cout<<hexa<<endl;
      cout<<"Do you trust the host key? [Y/n]"<<endl;
      ssh_string_free_char(hexa);
      ssh_clean_pubkey_hash(&hash);

      //Get response
      char buf[10];
      char* p = fgets(buf, sizeof(buf), stdin);
      if (p == NULL) {return;}
      string input(buf);
      if(input == "" || input == "y" || input == "Y" || input == "yes"){
        rc = ssh_session_update_known_hosts(ssh);
        if (rc < 0) {
          cout<< "Error "<<strerror(errno)<<endl;
          return;
        }
      }
      break;
    }
    case SSH_KNOWN_HOSTS_ERROR:{
      cout<< "Error "<<ssh_get_error(ssh)<<endl;
      ssh_clean_pubkey_hash(&hash);
      return;
    }
  }

  //---------------------------
  ssh_clean_pubkey_hash(&hash);
}
void SSH::ssh_autentification_user(){
  int nb_tentatives = 3;
  //---------------------------

  //With password
  bool success = true;
   for(int i=0; i<nb_tentatives; i++){
    char* password = getpass("Password: ");
    int rc = ssh_userauth_password(ssh, NULL, password);

    if (rc != SSH_AUTH_SUCCESS){
      cout<<"Error authenticating with password: "<<ssh_get_error(ssh)<<endl;
      cout<<"New tentative: "<<to_string(i + 1)<<"/"<<to_string(nb_tentatives)<<endl;
      success = false;
    }else{
      cout<<"Identification OK."<<endl;
      ssh_connected = true;
      success = true;
      break;
    }
  }

  //With public key
  /*int rc;
  rc = ssh_userauth_publickey_auto(ssh, NULL, NULL);
  if (rc == SSH_AUTH_ERROR){
   cout<< "Error authenticating with public key: %s\n", ssh_get_error(ssh));
 }*/

  if(success == false){
    cout<<"Exit program..."<<endl;
    exit(0);
  }

  //---------------------------
}
