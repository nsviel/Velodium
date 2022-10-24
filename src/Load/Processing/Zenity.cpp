#include "Zenity.h"

#include "../../Specific/fct_system.h"

#include <Eigen/Dense>
#include <set>
#include <string>
#include <filesystem>


//Constructor / destructor
Zenity::Zenity(){
  //---------------------------

  //---------------------------
}
Zenity::~Zenity(){}

//Zenity function
vector<string> Zenity::zenity_loading(string& path_current_dir, string title){
  //---------------------------

  //Open zenity file manager
  string zenity = "zenity --file-selection --multiple --title=" + title + " --filename=" + path_current_dir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  vector<string> path_vec;

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        string path_file = path_str.substr(0, path_str.find('|'));

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

    //Actualize current directory path
    string path = path_vec[0];
    path_current_dir = path.substr(0, path.find_last_of("/") + 1);
  }

  //---------------------------
  return path_vec;
}
string Zenity::zenity_saving(string& path_current_dir, string filename){
  //---------------------------

  string path_saving = "";
  string path = path_current_dir + filename +".pts";

  //Open Zenity window
  string zenity = "zenity --file-selection --save --title=Save --filename=" + path;
  FILE *file = popen(zenity.c_str(), "r");
  char path_buffer[1024];
  char* path_char = fgets(path_buffer, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    //Supress unwanted line break
    string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_saving = path_str;
  }

  //Set current directory
  path_current_dir = path_saving.substr(0, path_saving.find_last_of("/") + 1);

  //---------------------------
  return path_saving;
}
string Zenity::zenity_directory(string& path_current_dir){
  string path_directory = "";
  //---------------------------

  //Retrieve dir path
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_current_dir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    //Remove unwanted break line
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_directory = path_str;
  }

  //Set current directory
  path_current_dir = path_directory;

  //---------------------------
  return path_directory;
}
void Zenity::zenity_select_directory(string& path_dir){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_dir;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
    }

    path_dir = path_str + "/";
  }

  //---------------------------
}
