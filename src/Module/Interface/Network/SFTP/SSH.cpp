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

    // Authenticate server
    bool server_connected = ssh_autentification_server();

    // Authenticate user
    bool user_connected = ssh_autentification_user();

    //Set connection flag on
    if(server_connected && user_connected){
      ssh_connected = true;
      string log = "Connected to " + selected_ip + " with SSH";
      console.AddLog("ok", log);
    }
    else{
      string log = "Problem connection SSH";
      console.AddLog("error", log);
    }
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
bool SSH::ssh_autentification_server(){
  //SSH host file: (~/.ssh/known_hosts on UNIX)
  //---------------------------

  bool success = true;
  unsigned char *hash = NULL;
  ssh_key srv_pubkey = NULL;
  size_t hlen;
  int rc;

  //Public key stuff
  rc = ssh_get_server_publickey(ssh, &srv_pubkey);
  if (rc < 0) {return false;}
  rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  if (rc < 0) {return false;}
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
      return false;
    }
    case SSH_KNOWN_HOSTS_OTHER:{
      cout<< "The host key for this server was not found but an other type of key exists."<<endl;
      cout<< "An attacker might change the default server key to confuse your client into thinking the key does not exist"<<endl;
      ssh_clean_pubkey_hash(&hash);
      return false;
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
      if (p == NULL) {return false;}
      string input(buf);
      if(input == "" || input == "y" || input == "Y" || input == "yes"){
        rc = ssh_session_update_known_hosts(ssh);
        if (rc < 0) {
          cout<< "Error "<<strerror(errno)<<endl;
          return false;
        }
      }

      //Feedback success
      cout<<"Host key added"<<endl;
      break;
    }
    case SSH_KNOWN_HOSTS_ERROR:{
      cout<< "Error "<<ssh_get_error(ssh)<<endl;
      ssh_clean_pubkey_hash(&hash);
      return false;
    }
  }

  ssh_clean_pubkey_hash(&hash);

  //---------------------------
  return true;
}
bool SSH::ssh_autentification_user(){
  //---------------------------

  //Check if no user identification is ok
  int rc = ssh_userauth_none(ssh, NULL);
  if (rc == SSH_AUTH_SUCCESS || rc == SSH_AUTH_ERROR) {
    return true;
  }

  //Else check other identification methods
  int method = ssh_userauth_list(ssh, NULL);

  if (method & SSH_AUTH_METHOD_PUBLICKEY){
    rc = authenticate_pubkey();
    if (rc == SSH_AUTH_SUCCESS) return true;
  }
  if (method & SSH_AUTH_METHOD_INTERACTIVE){
    rc = authenticate_kbdint();
    if (rc == SSH_AUTH_SUCCESS) return true;
  }
  if (method & SSH_AUTH_METHOD_PASSWORD){
    rc = authenticate_password();
    if (rc == SSH_AUTH_SUCCESS) return true;
  }

  //---------------------------
  return false;
}

int SSH::authenticate_pubkey(){
  //---------------------------

  int rc = ssh_userauth_publickey_auto(ssh, NULL, NULL);
  if (rc == SSH_AUTH_ERROR){
     fprintf(stderr, "Authentication failed: %s\n",
       ssh_get_error(ssh));
     return SSH_AUTH_ERROR;
  }

  //---------------------------
  return rc;
}
int SSH::authenticate_kbdint(){
  //---------------------------

  int rc = ssh_userauth_kbdint(ssh, NULL, NULL);
  while (rc == SSH_AUTH_INFO){
    const char *name, *instruction;
    int nprompts, iprompt;

    name = ssh_userauth_kbdint_getname(ssh);
    instruction = ssh_userauth_kbdint_getinstruction(ssh);
    nprompts = ssh_userauth_kbdint_getnprompts(ssh);

    if (strlen(name) > 0)
      printf("%s\n", name);
    if (strlen(instruction) > 0)
      printf("%s\n", instruction);
    for (iprompt = 0; iprompt < nprompts; iprompt++){
      const char *prompt;
      char echo;

      prompt = ssh_userauth_kbdint_getprompt(ssh, iprompt, &echo);
      if (echo){
        char buffer[128], *ptr;

        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
          return SSH_AUTH_ERROR;
        buffer[sizeof(buffer) - 1] = '\0';
        if ((ptr = strchr(buffer, '\n')) != NULL)
          *ptr = '\0';
        if (ssh_userauth_kbdint_setanswer(ssh, iprompt, buffer) < 0)
          return SSH_AUTH_ERROR;
        memset(buffer, 0, strlen(buffer));
      }
      else
      {
        char *ptr;

        ptr = getpass(prompt);
        if (ssh_userauth_kbdint_setanswer(ssh, iprompt, ptr) < 0)
          return SSH_AUTH_ERROR;
      }
    }
    rc = ssh_userauth_kbdint(ssh, NULL, NULL);
  }

  //---------------------------
  return rc;
}
int SSH::authenticate_password(){
  int nb_tentatives = 3;
  int rc;
  //---------------------------

  //With password
  bool success = true;
  for(int i=0; i<nb_tentatives; i++){
    char* password = getpass("Password: ");
    int rc = ssh_userauth_password(ssh, NULL, password);

    if (rc != SSH_AUTH_SUCCESS){
      cout<<"Error authenticating with password: "<<ssh_get_error(ssh)<<endl;
      cout<<"New tentative: "<<to_string(i + 1)<<"/"<<to_string(nb_tentatives)<<endl;
    }else{
      cout<<"Identification OK."<<endl;
      break;
    }
  }

  //---------------------------
  return rc;
}
