#include "Network.h"

#include "../../Specific/fct_maths.h"
#include "../../Operation/Operation.h"


//Constructor / Destructor
Network::Network(){
  //---------------------------

  this->ssh_port = 22;
  this->ssh_adress_ID = 0;
  this->ssh_connected = false;
  this->path_source = "/home/aither/Desktop/frame_0001.ply";
  this->path_target = "/home/aither/Desktop/apple/";
  this->name_file = "frame_0001.ply";

  //---------------------------
}
Network::~Network(){
  //---------------------------

  //Desallocate
  this->ssh_stopConnexion();

  //---------------------------
}

void Network::ssh_connexion(){
  //---------------------------

  //SSH allocation
  this->ssh_startConnexion();

  // Verify the server's identity
  this->ssh_verify_knownhost();

  // Authenticate ourselves
  this->ssh_autentification();

  //---------------------------
}
void Network::sftp_sendFile(){
  //---------------------------

  //Check if ssh is established
  if(ssh_connected == false){
    cout<<"SSH not connected."<<endl;
    return;
  }

  sftp_session sftp = sftp_new(SSH);
  if (sftp == NULL){
    fprintf(stderr, "Error allocating SFTP SSH: %s\n", ssh_get_error(SSH));
    return;
  }
  int rc = sftp_init(sftp);
  if (rc != SSH_OK){
    fprintf(stderr, "Error initializing SFTP SSH.\n");
    sftp_free(sftp);
    return;
  }

  //Select the file where we are writting
  int access_type = O_WRONLY | O_CREAT | O_TRUNC;
  string path = path_target + name_file;
  sftp_file file = sftp_open(sftp, path.c_str(), access_type, S_IRWXU);
  if (file == NULL){
    fprintf(stderr, "Can't open file for writing: %s\n",
            ssh_get_error(SSH));
    return;
  }

  //Open local file and write on remote
  say(path_source);
  ifstream fin(path_source, ios::binary);
  while (fin){
    constexpr size_t max_xfer_buf_size = 10240;
    char buffer[max_xfer_buf_size];
    fin.read(buffer, sizeof(buffer));
    if (fin.gcount() > 0){
      ssize_t nwritten = sftp_write(file, buffer, fin.gcount());
      if (nwritten != fin.gcount()){
        fprintf(stderr, "Error writing to file: %s\n", ssh_get_error(SSH));
        sftp_close(file);
        return;
      }
    }
  }

  //Close and stop transfert
  rc = sftp_close(file);
  if (rc != SSH_OK){
    fprintf(stderr, "Can't close the written file: %s\n",
            ssh_get_error(SSH));
    return;
  }

  //---------------------------
  sftp_free(sftp);
}
void Network::sftp_createDirectory(string dirName){
  //---------------------------

  //Check if ssh is established
  if(ssh_connected == false){
    cout<<"SSH not connected."<<endl;
    return;
  }

  sftp_session sftp = sftp_new(SSH);
  if (sftp == NULL){
    fprintf(stderr, "Error allocating SFTP SSH: %s\n", ssh_get_error(SSH));
    return;
  }
  int rc = sftp_init(sftp);
  if (rc != SSH_OK){
    fprintf(stderr, "Error initializing SFTP SSH.\n");
    sftp_free(sftp);
    return;
  }

  //Create new directory
  rc = sftp_mkdir(sftp, dirName.c_str(), S_IRWXU);
  if (rc != SSH_OK){
    fprintf(stderr, "Can't create directory.\n");
  }

  //---------------------------
}

void Network::ssh_startConnexion(){
  //---------------------------

  //SSH allocation
  SSH = ssh_new();

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
  ssh_options_set(SSH, SSH_OPTIONS_HOST, ssh_adress.c_str());
  ssh_options_set(SSH, SSH_OPTIONS_PORT, &ssh_port);

  int rc = ssh_connect(SSH);
  if (rc != SSH_OK){
    fprintf(stderr, "Error connecting to localhost: %s\n", ssh_adress.c_str());
    return;
  }

  //---------------------------
}
void Network::ssh_stopConnexion(){
  //---------------------------

  ssh_disconnect(SSH);
  ssh_free(SSH);
  this->ssh_connected = false;

  //---------------------------
}
void Network::ssh_autentification(){
  //---------------------------

  char* password = getpass("Password: ");
  int rc = ssh_userauth_password(SSH, NULL, password);
  if (rc != SSH_AUTH_SUCCESS){
    fprintf(stderr, "Error authenticating with password: %s\n", ssh_get_error(SSH));
    ssh_disconnect(SSH);
    ssh_free(SSH);
    exit(-1);
  }else{
    cout<<"Identification OK."<<endl;
    ssh_connected = true;
  }

  //---------------------------
}
void Network::ssh_verify_knownhost(){
  //---------------------------

  bool sucess = true;
  unsigned char *hash = NULL;
  ssh_key srv_pubkey = NULL;
  size_t hlen;
  int rc;

  //Public key stuff
  rc = ssh_get_server_publickey(SSH, &srv_pubkey);
  if (rc < 0) {
    return;
  }
  rc = ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
  if (rc < 0) {
    return;
  }
  ssh_key_free(srv_pubkey);

  //Check is the server is known
  enum ssh_known_hosts_e state = ssh_session_is_known_server(SSH);
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
      rc = ssh_session_update_known_hosts(SSH);
      if (rc < 0) {
        fprintf(stderr, "Error %s\n", strerror(errno));
        return;
      }
      break;
    }
    case SSH_KNOWN_HOSTS_ERROR:{
      fprintf(stderr, "Error %s", ssh_get_error(SSH));
      ssh_clean_pubkey_hash(&hash);
      return;
    }
  }

  //---------------------------
  ssh_clean_pubkey_hash(&hash);
}

void Network::select_fileToSend(){
  //---------------------------

  //Select file to send
  Operation opeManager;
  path_source = opeManager.get_filePath();

  //Supress unwanted line break
  if (path_source.find('\n')){
    path_source.erase(std::remove(path_source.begin(), path_source.end(), '\n'), path_source.end());
  }

  //Retrieve name file
  name_file = path_source.substr(path_source.find_last_of("/\\") + 1);

  //Supress unwanted line break
  if (name_file.find('\n')){
    name_file.erase(std::remove(name_file.begin(), name_file.end(), '\n'), name_file.end());
  }

  //---------------------------
}
