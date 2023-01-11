#include "Zenity.h"
#include "Directory.h"
#include "Path.h"


std::vector<std::string> zenity_file_vec(std::string title, std::string& path_dir){
  //---------------------------

  //Open zenity file manager
  std::string path_current_dir = get_path_abs_build() + '/';
  std::string zenity = "zenity --file-selection --multiple --title=" + title + " --filename=" + path_dir + " 2> /dev/null";

  FILE *file = popen(zenity.c_str(), "r");
  char filename[1000000];
  const char* path_char = fgets(filename, 1000000, file);
  std::vector<std::string> path_vec;

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    std::string path_str(path_char);

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        std::string path_file = path_str.substr(0, path_str.find('|'));

        if (path_file.find('\n')){
          path_file.erase(std::remove(path_file.begin(), path_file.end(), '\n'), path_file.end());
        }

        path_vec.push_back(path_file);

        //Supress retrieved path from path list
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }

    //Retrieve the last selected path
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
      path_dir = path_str;
    }
    path_vec.push_back(path_str);
  }

  //---------------------------
  return path_vec;
}
std::vector<std::string> zenity_file_vec(std::string title){
  //---------------------------

  //Open zenity file manager
  std::string path_current_dir = get_path_abs_build() + '/';
  std::string zenity = "zenity --file-selection --multiple --title=" + title + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  std::vector<std::string> path_vec;

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    std::string path_str(path_char);

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        std::string path_file = path_str.substr(0, path_str.find('|'));

        if (path_file.find('\n')){
          path_file.erase(std::remove(path_file.begin(), path_file.end(), '\n'), path_file.end());
        }

        path_vec.push_back(path_file);

        //Supress retrieved path from path list
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }

    //Retrieve the last selected path
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_vec.push_back(path_str);
  }

  //---------------------------
  return path_vec;
}
std::string zenity_directory(std::string title, std::string& path_dir){
  std::string path_directory = "";
  //---------------------------

  //Retrieve dir path
  std::string zenity = "zenity --file-selection --directory --title=" + title + " --filename=" + path_dir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    std::string path_str(path_char);

    //Remove unwanted break line
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_directory = path_str;
  }

  //---------------------------
  return path_directory;
}
void zenity_file(std::string& path_file){
  std::string path_saving = "";
  //---------------------------

  //Open Zenity window
  std::string zenity = "zenity --file-selection --save --filename=" + path_file;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    std::string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_file = path_str;
  }

  //---------------------------
}
std::string zenity_file(){
  std::string path_saving = "";
  //---------------------------

  //Open Zenity window
  std::string zenity = "zenity --file-selection --save";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    std::string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_saving = path_str;
  }

  //---------------------------
  return path_saving;
}
void zenity_directory(std::string& path_dir){
  //---------------------------

  //Get absolute executable location
  std::string zenity = "zenity --file-selection --directory --title='Select directory' --filename=" + path_dir;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    std::string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_dir = path_str + "/";
  }

  //---------------------------
}
std::string zenity_saving(std::string& dir, std::string filename){
  //---------------------------

  std::string path = dir + filename;
  std::string path_saving = "";

  //Open Zenity window
  std::string zenity = "zenity --file-selection --save --title=Save --filename=" + path;
  FILE *file = popen(zenity.c_str(), "r");
  char path_buffer[1024];
  char* path_char = fgets(path_buffer, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    //Supress unwanted line break
    std::string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_saving = path_str;
  }

  //Set current directory
  dir = path_saving.substr(0, path_saving.find_last_of("/") + 1);

  //---------------------------
  return path_saving;
}
