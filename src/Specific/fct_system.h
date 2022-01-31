#ifndef SYSTEM_FUNCTIONS_H
#define SYSTEM_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <random>
#include <fstream>
#include <unistd.h>
#include <experimental/filesystem>

/**
 * \namespace OpenGL functions
 * \brief Specific OpenGL functions
 */

namespace{
  //---------------------------

  //Get RAM memory usage
  unsigned long long get_systemMemory(){
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);
    return pages * page_size;
  }

  //Get absolute build path
  string get_absolutePath_build(){
    string absPath = std::experimental::filesystem::current_path();
    return absPath;
  }

  //Get size of file - number of points
  int get_fileSize(std::string pathFile){
    int cpt=0;
    std::string line;
    std::ifstream infile(pathFile);
    while (std::getline(infile, line)){
      cpt++;
    }
    return cpt;
  }

  //Random selection vector - int r = *select_randomly(foo.begin(), foo.end());
  template<typename Type, typename RandomGenerator> Type select_randomly(Type start, Type end, RandomGenerator& g) {
      std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
      std::advance(start, dis(g));
      return start;
  }
  template<typename Type> Type select_randomly(Type start, Type end){
      static std::random_device rd;
      static std::mt19937 gen(rd());
      return select_randomly(start, end, gen);
  }

  //File management
  std::vector<std::string> list_allFiles(const char *path){
      struct dirent* files;
      DIR* directory = opendir(path);
      std::vector<std::string> list;

      //Filtre and store files present in the folder
      while ((files = readdir(directory)) != NULL)
      {
          std::string name = files->d_name;
          if(name.find(".pts") != std::string::npos || name.find(".ply") != std::string::npos || name.find(".ptx") != std::string::npos || name.find(".obj") != std::string::npos)
          {
            list.push_back(name);
          }
      }

      //Close and return the file names list
      closedir(directory);
      return list;
  }
  std::vector<std::string> list_allDirs(const char *path){
    struct dirent* files;
    DIR* directory = opendir(path);
    std::vector<std::string> list;

    //Filtre and store files present in the folder
    while ((files = readdir(directory)) != NULL)
    {
        std::string name = files->d_name;
        if((strchr(files->d_name, '.')) == NULL)
        {
          list.push_back(name + "/");
        }
    }

    //Close and return the file names list
    closedir(directory);
    return list;
  }
  int strcasecmp_withNumbers(const char *void_a, const char *void_b) {
   const char *a = void_a;
   const char *b = void_b;

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
   return *a ? 1 : *b ? -1 : 0;
 }
  int get_dir_numberOfFile(std::string path){
    DIR *dp;
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

    return i;
 }
  bool is_file_exist(std::string& fileName){
    //---------------------------

    std::ifstream infile(fileName.c_str());

    //---------------------------
    return infile.good();
  }
  void clean_directory_files(const char *path){
    std::vector<std::string> path_vec = list_allFiles(path);
    for(int i=0; i<path_vec.size(); i++){
      string path_full = path + path_vec[i];
      std::remove (path_full.c_str());
    }
  }

  //---------------------------
}

#endif
