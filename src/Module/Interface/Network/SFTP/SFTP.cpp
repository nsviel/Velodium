#include "SFTP.h"

#include "../../../../Specific/fct_maths.h"


//Constructor / Destructor
SFTP::SFTP(){
  //---------------------------



  //---------------------------
}
SFTP::~SFTP(){}

void SFTP::sftp_sendFile(ssh_session& ssh, string path_src, string path_trg){
  sftp = sftp_new(ssh);
  //---------------------------

  //CHeck SFTP connexion
  if (sftp == NULL){
    fprintf(stderr, "Error allocating SFTP SSH: %s\n", ssh_get_error(ssh));
    return;
  }

  //Init SFTP
  int rc = sftp_init(sftp);
  if (rc != SSH_OK){
    fprintf(stderr, "Error initializing SFTP SSH.\n");
    sftp_free(sftp);
    return;
  }

  //Select the file where we are writting
  int access_type = O_WRONLY | O_CREAT | O_TRUNC;
  sftp_file file = sftp_open(sftp, path_trg.c_str(), access_type, S_IRWXU);
  if (file == NULL){
    fprintf(stderr, "Can't open file for writing: %s\n", ssh_get_error(ssh));
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
        fprintf(stderr, "Error writing to file: %s\n", ssh_get_error(ssh));
        sftp_close(file);
        return;
      }
    }
  }

  //Close and stop transfert
  rc = sftp_close(file);
  if (rc != SSH_OK){
    fprintf(stderr, "Can't close the written file: %s\n", ssh_get_error(ssh));
    return;
  }

  //---------------------------
  sftp_free(sftp);
}
void SFTP::sftp_createDirectory(ssh_session& ssh, string dirName){
  sftp = sftp_new(ssh);
  //---------------------------

  //Check STFP connexion
  if (sftp == NULL){
    fprintf(stderr, "Error allocating SFTP SSH: %s\n", ssh_get_error(ssh));
    return;
  }

  //Init STFP connexion
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
