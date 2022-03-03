#include "SSH.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / Destructor
SSH::SSH(){
  //---------------------------

  this->ssh_port = 22;
  this->ssh_adress_ID = 0;
  this->ssh_connected = false;

  //---------------------------
}
SSH::~SSH(){
  //---------------------------

  this->ssh_stopConnexion();

  //---------------------------
}

void SSH::ssh_startConnexion(){
  //---------------------------

  //SSH allocation
  ssh = ssh_new();

  //SSH adressing
  switch(ssh_adress_ID){
    case 0:{//Localhost
      this->ssh_adress = "127.0.0.1";
      break;
    }
    case 1:{//Ordi Louis
      this->ssh_adress = "10.201.20.110";
      break;
    }
  }

  //Options
  ssh_options_set(ssh, SSH_OPTIONS_HOST, ssh_adress.c_str());
  ssh_options_set(ssh, SSH_OPTIONS_PORT, &ssh_port);

  int rc = ssh_connect(ssh);
  if (rc != SSH_OK){
    fprintf(stderr, "Error connecting to localhost: %s\n", ssh_adress.c_str());
    return;
  }

  // Verify the server's identity
  this->ssh_verify_knownhost();

  // Authenticate ourselves
  this->ssh_autentification();

  //---------------------------
}
void SSH::ssh_stopConnexion(){
  //---------------------------

  ssh_disconnect(ssh);
  ssh_free(ssh);
  this->ssh_connected = false;

  //---------------------------
}
void SSH::ssh_autentification(){
  int nb_tentatives = 3;
  //---------------------------

  //With password
  bool sucess = true;
   for(int i=0; i<nb_tentatives; i++){
    char* password = getpass("Password: ");
    int rc = ssh_userauth_password(ssh, NULL, password);

    if (rc != SSH_AUTH_SUCCESS){
      fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(ssh));
      cout<<"New tentative: "<<to_string(i + 1)<<"/"<<to_string(nb_tentatives)<<endl;
      sucess = false;
    }else{
      cout<<"Identification OK."<<endl;
      ssh_connected = true;
      sucess = true;
      break;
    }
  }

  //With public key
  /*int rc;
  rc = ssh_userauth_publickey_auto(ssh, NULL, NULL);
  if (rc == SSH_AUTH_ERROR){
   fprintf(stderr, "Error authenticating with public key: %s\n", ssh_get_error(ssh));
 }*/

  if(sucess == false){
    cout<<"Exit program..."<<endl;
    exit(0);
  }

  //---------------------------
}
void SSH::ssh_verify_knownhost(){
  //SSH host file: (~/.ssh/known_hosts on UNIX)
  //---------------------------

  bool sucess = true;
  unsigned char *hash = NULL;
  ssh_key srv_pubkey = NULL;
  size_t hlen;
  int rc;

  //Public key stuff
  rc = ssh_get_server_publickey(ssh, &srv_pubkey);
  if (rc < 0) {
    return;
  }
  rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  if (rc < 0) {
    return;
  }
  ssh_key_free(srv_pubkey);

  //Check is the server is known
  enum ssh_known_hosts_e state = ssh_session_is_known_server(ssh);
  switch (state) {
    case SSH_KNOWN_HOSTS_OK:{
      cout<<"Server is known."<<endl;
      break;
    }
    case SSH_KNOWN_HOSTS_CHANGED:{
      fprintf(stderr, "Host key for server changed\n");
      fprintf(stderr, "For security reasons, connection will be stopped\n");
      ssh_clean_pubkey_hash(&hash);
      return;
    }
    case SSH_KNOWN_HOSTS_OTHER:{
      fprintf(stderr, "The host key for this server was not found but an other"
              "type of key exists.\n");
      fprintf(stderr, "An attacker might change the default server key to"
              "confuse your client into thinking the key does not exist\n");
      ssh_clean_pubkey_hash(&hash);
      return;
    }
    case SSH_KNOWN_HOSTS_NOT_FOUND:{
      fprintf(stderr, "Could not find known host file.\n");
      fprintf(stderr, "If you accept the host key here, the file will be"
              "automatically created.\n");
      /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */
    }
    case SSH_KNOWN_HOSTS_UNKNOWN:{
      char buf[10];
      char* hexa = ssh_get_hexa(hash, hlen);

      fprintf(stderr,"The server is unknown. Do you trust the host key?\n");
      fprintf(stderr, "Public key hash: %s\n", hexa);
      ssh_string_free_char(hexa);
      ssh_clean_pubkey_hash(&hash);
      char* p = fgets(buf, sizeof(buf), stdin);
      if (p == NULL) {
        return;
      }
      int cmp = strncasecmp(buf, "yes", 3);
      if (cmp != 0) {
        return;
      }
      rc = ssh_session_update_known_hosts(ssh);
      if (rc < 0) {
        fprintf(stderr, "Error %s\n", strerror(errno));
        return;
      }
      break;
    }
    case SSH_KNOWN_HOSTS_ERROR:{
      fprintf(stderr, "Error %s", ssh_get_error(ssh));
      ssh_clean_pubkey_hash(&hash);
      return;
    }
  }

  //---------------------------
  ssh_clean_pubkey_hash(&hash);
}
