#include "SFTP.h"

#include "SSH.h"
#include "../Network.h"

#include "../../../../Specific/fct_maths.h"
#include "../../../../Specific/fct_system.h"


//Constructor / Destructor
SFTP::SFTP(Network* netManager){
  //---------------------------

  this->sshManager = netManager->get_sshManager();
  this->sftp_connected = false;

  //---------------------------
}
SFTP::~SFTP(){}

void SFTP::sftp_new_session(){
  //---------------------------

  if(sshManager->get_ssh_connected() && sftp_connected == false){
    ssh_session ssh = sshManager->get_ssh_session();

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

    //Connection established
    this->sftp_connected = true;
    string log = "Connected with SFTP";
    console.AddLog("ok", log);
  }

  //---------------------------
}
void SFTP::sftp_disconnection(){
  //---------------------------

  if(sftp_connected){
    sftp_free(sftp);
    this->sftp_connected = false;
  }

  //---------------------------
}

void SFTP::sftp_send_file(string path_src, string path_trg, string file_name){
  if(sftp_connected){
    ssh_session ssh = sshManager->get_ssh_session();
    //---------------------------

    //Check if source file exists
    if(is_file_exist(path_src) == false){
      cout<<"File "<<path_src<<" doesn't exists"<<endl;
      return;
    }

    //Select the file where we are writting
    int access_type = O_WRONLY | O_CREAT | O_TRUNC;
    string path_file = path_trg + file_name;
    sftp_file file = sftp_open(sftp, path_file.c_str(), access_type, S_IRWXU);
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
    int rc = sftp_close(file);
    if (rc != SSH_OK){
      cout<<"Can't close the written file: "<<ssh_get_error(ssh)<<endl;
      return;
    }

    //Display success
    string log = "File " + path_src + " sended to " + path_file;
    console.AddLog("ok", log);

    //---------------------------
  }else{
    console.AddLog("error", "SFTP not connected");
  }
}
void SFTP::sftp_create_directory(string dirName){
  if(sftp_connected){
    //---------------------------

    //Create new directory
    int rc = sftp_mkdir(sftp, dirName.c_str(), S_IRWXU);
    if (rc != SSH_OK){
      cout<<"[SFTP] Can't create directory."<<endl;
    }

    //---------------------------
  }
}
