#ifndef SYSTEM_FUNCTIONS_H
#define SYSTEM_FUNCTIONS_H

/**
 * \namespace System functions
 * \brief Specific system functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/stat.h>
#include <dirent.h>
#include <random>
#include <fstream>
#include <unistd.h>
#include <filesystem>
#include <experimental/filesystem>


namespace{
  //---------------------------

  //New directory creation
  void create_new_dir(std::string path){
    //---------------------------

    int last = path.find_last_of("/");
    std::string dir = path.substr(0, last);
    last = dir.find_last_of("/");
    dir = dir.substr(0, last);

    if(std::filesystem::exists(dir) == true && std::filesystem::exists(path) == false){
      std::filesystem::create_directory(path);
    }

    //---------------------------
  }

  //Retrieve file format (eg, pts, json, pcap)
  std::string get_file_format(std::string path){
    //---------------------------

    std::string format = path.substr(path.find_last_of("."), string::npos);

    //---------------------------
    return format;
  }

  //Get a RGB random color
  glm::vec4 random_color(){
    //---------------------------

    float Red = float(rand()%101)/100;
    float Green = float(rand()%101)/100;
    float Blue = float(rand()%101)/100;
    glm::vec4 color = glm::vec4(Red, Green, Blue, 1.0f);

    //---------------------------
    return color;
  }

  //Get RAM memory usage
  unsigned long long get_systemMemory(){
    //---------------------------

    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);

    //---------------------------
    return pages * page_size;
  }

  //Get absolute build path
  string get_absolutePath_build(){
    //---------------------------

    string absPath = std::experimental::filesystem::current_path();
    absPath += "/";

    //---------------------------
    return absPath;
  }

  //Get size of file - number of points
  int get_fileSize(std::string pathFile){
    //---------------------------

    int cpt=0;
    std::string line;
    std::ifstream infile(pathFile);
    while (std::getline(infile, line)){
      cpt++;
    }

    //---------------------------
    return cpt;
  }

  //Random selection vector - int r = *select_randomly(foo.begin(), foo.end());
  template<typename Type, typename RandomGenerator> Type select_randomly(Type start, Type end, RandomGenerator& g){
    //---------------------------

    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));

    //---------------------------
    return start;
  }
  template<typename Type> Type select_randomly(Type start, Type end){
    //---------------------------

    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);

    //---------------------------
  }

  //File management
  bool is_file_exist(std::string& fileName){
    //---------------------------

    std::ifstream infile(fileName.c_str());

    //---------------------------
    return infile.good();
  }
  bool is_dir_exist(std::string& path){
    //---------------------------

    if(std::filesystem::exists(path)){
      return true;
    }else{
      return false;
    }

    //---------------------------
  }
  std::vector<std::string> list_allFiles(std::string path){
    //---------------------------

    struct dirent* files;
    DIR* directory = opendir(path.c_str());
    std::vector<std::string> path_vec;

    if(is_dir_exist(path) == false){
      std::cout<<"[error] Directory does not exists: "<<path<<std::endl;
      return path_vec;
    }

    //Filtre and store files present in the folder
    while ((files = readdir(directory)) != NULL){
      std::string name = files->d_name;

      if(name != "." && name != ".."){
        path_vec.push_back(name);
      }
    }

    //Close and return the file names list
    closedir(directory);

    //Sort vector in alphabetic order
    std::sort(path_vec.begin(), path_vec.end());

    //---------------------------
    return path_vec;
  }
  std::vector<std::string> list_allPaths(string path_dir){
    //---------------------------

    struct dirent* files;
    DIR* directory = opendir(path_dir.c_str());
    std::vector<std::string> path_vec;

    //Check if directory exists
    if(is_dir_exist(path_dir) == false){
      std::cout<<"[error] Directory does not exists: "<<path_dir<<std::endl;
      return path_vec;
    }

    //Supress unwanted line break
    if(path_dir.find('\n')){
      path_dir.erase(std::remove(path_dir.begin(), path_dir.end(), '\n'), path_dir.end());
    }

    //Filtre and store files present in the folder
    while ((files = readdir(directory)) != NULL){
      std::string path_file = files->d_name;
      std::string path_full = path_dir + "/" + path_file;

      if(path_file != "." && path_file != ".."){
        path_vec.push_back(path_full);
      }
    }

    //Close and return the file names list
    closedir(directory);

    //Sort vector in alphabetic order
    std::sort(path_vec.begin(), path_vec.end());

    //---------------------------
    return path_vec;
  }
  std::vector<std::string> list_allDirs(const char *path){
    //---------------------------

    struct dirent* files;
    DIR* directory = opendir(path);
    std::vector<std::string> list;

    //Filtre and store files present in the folder
    while ((files = readdir(directory)) != NULL){
      std::string name = files->d_name;
      if((strchr(files->d_name, '.')) == NULL){
        list.push_back(name + "/");
      }
    }

    //Close and return the file names list
    closedir(directory);

    //---------------------------
    return list;
  }
  int strcasecmp_withNumbers(const char *void_a, const char *void_b) {
    const char *a = void_a;
    const char *b = void_b;
    //---------------------------

    if (!a || !b) { // if one doesn't exist, other wins by default
      return a ? 1 : b ? -1 : 0;
    }
    if (isdigit(*a) && isdigit(*b)) { // if both start with numbers
      char *remainderA;
      char *remainderB;
      long valA = strtol(a, &remainderA, 10);
      long valB = strtol(b, &remainderB, 10);
      if (valA != valB)
         return valA - valB;
      // if you wish 7 == 007, comment out the next two lines
      else if (remainderB - b != remainderA - a) // equal with diff lengths
         return (remainderB - b) - (remainderA - a); // set 007 before 7
      else // if numerical parts equal, recurse
         return strcasecmp_withNumbers(remainderA, remainderB);
    }
    if (isdigit(*a) || isdigit(*b)) { // if just one is a number
      return isdigit(*a) ? -1 : 1; // numbers always come first
    }
    while (*a && *b) { // non-numeric characters
      if (isdigit(*a) || isdigit(*b))
         return strcasecmp_withNumbers(a, b); // recurse
      if (tolower(*a) != tolower(*b))
         return tolower(*a) - tolower(*b);
      a++;
      b++;
    }

    //---------------------------
    return *a ? 1 : *b ? -1 : 0;
 }
  int get_dir_numberOfFile(std::string path){
    DIR *dp;
    //---------------------------

    int i = 0;
    struct dirent *ep;
    dp = opendir (path.c_str());

    if (dp != NULL){
     while (ep = readdir (dp)){
       i++;
     }
     (void) closedir (dp);
    }
    else{
     perror ("Couldn't open the directory");
    }

    //Since ./ and ../ are counted
    i = i-2;

    //---------------------------
    return i;
 }
  void clean_directory_files(const char *path){
    struct stat buffer;
    if(stat (path, &buffer) != 0) return;
    //---------------------------

    std::vector<std::string> path_vec = list_allFiles(path);
    for(int i=0; i<path_vec.size(); i++){
      string path_full = path + path_vec[i];
      std::remove (path_full.c_str());
    }

    //---------------------------
  }

  //---------------------------
}

#endif
