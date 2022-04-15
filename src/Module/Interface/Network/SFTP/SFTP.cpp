#include "SFTP.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / Destructor
SFTP::SFTP(){
  //---------------------------

  this->sftp_connected = false;

  //---------------------------
}
SFTP::~SFTP(){}

void SFTP::sftp_send_file(ssh_session& ssh, string path_src, string path_trg){
  //---------------------------

  //Check connection
  if(sftp_connected == false){
    cout<<"SFTP not connected"<<endl;
    return;
  }

  //Select the file where we are writting
  int access_type = O_WRONLY | O_CREAT | O_TRUNC;
  sftp_file file = sftp_open(sftp, path_trg.c_str(), access_type, S_IRWXU);
  if (file == NULL){
    cout<<"Can't open file for writing: "<<ssh_get_error(ssh)<<endl;
    return;
  }

  //Open local file and write on remote
  ifstream fin(path_src, ios::binary);
  while (fin){
    constexpr size_t max_xfer_buf_size = 10240;
    char buffer[max_xfer_buf_size];
    fin.read(buffer, sizeof(buffer));
    if (fin.gcount() > 0){
      ssize_t nwritten = sftp_write(file, buffer, fin.gcount());
      if (nwritten != fin.gcount()){
        cout<<"Error writing to file: "<<ssh_get_error(ssh)<<endl;
        sftp_close(file);
        return;
      }
    }
  }

  //Close and stop transfert
  rc = sftp_close(file);
  if (rc != SSH_OK){
    cout<<"Can't close the written file: "<<ssh_get_error(ssh)<<endl;
    return;
  }

  //---------------------------
}
void SFTP::sftp_create_directory(ssh_session& ssh, string dirName){
  //---------------------------

  //Check connection
  if(sftp_connected == false){
    cout<<"SFTP not connected"<<endl;
    return;
  }

  //Create new directory
  rc = sftp_mkdir(sftp, dirName.c_str(), S_IRWXU);
  if (rc != SSH_OK){
    cout<<"[SFTP] Can't create directory."<<endl;
  }

  //---------------------------
}

void SFTP::sftp_new_session(){
  //---------------------------

  //Start new session
  this->sftp = sftp_new(ssh);
  if(sftp == NULL){
    cout<<"Error allocating SFTP SSH: "<<ssh_get_error(ssh)<<endl;
    return;
  }

  //Init SFTP
  int rc = sftp_init(sftp);
  if (rc != SSH_OK){
    cout<<"Error initializing SFTP SSH."<<endl;
    sftp_free(sftp);
    return;
  }

  //---------------------------
  this->sftp_connected = true;
}
void SFTP::sftp_disconnection(){
  //---------------------------

  if(sftp_connected){
    sftp_free(sftp);
    this->sftp_connected = false;
  }

  //---------------------------
}
