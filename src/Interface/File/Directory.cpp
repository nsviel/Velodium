#include "Directory.h"


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
void clean_directory_files(const char *path){
  struct stat buffer;
  if(stat (path, &buffer) != 0) return;
  //---------------------------

  std::vector<std::string> path_vec = list_allFiles(path);
  for(int i=0; i<path_vec.size(); i++){
    std::string path_full = path + path_vec[i];
    std::remove (path_full.c_str());
  }

  //---------------------------
}

bool is_dir(std::string path){
  //---------------------------

  if(std::filesystem::is_directory(path)){
    return true;
  }else{
    return false;
  }

  //---------------------------
}
bool is_file_exist(std::string fileName){
  //---------------------------

  std::ifstream infile(fileName.c_str());

  //---------------------------
  return infile.good();
}
bool is_dir_exist(std::string path){
  //---------------------------

  if(std::filesystem::exists(path)){
    return true;
  }else{
    return false;
  }

  //---------------------------
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
std::vector<std::string> list_allPaths(std::string path_dir){
  //---------------------------

  struct dirent* files;
  DIR* directory = opendir(path_dir.c_str());
  std::vector<std::string> path_vec;

  //Check if directory exists
  if(is_dir_exist(path_dir) == false){
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
std::vector<std::string> list_allDirs(std::string path){
  //---------------------------

  struct dirent* files;
  DIR* directory = opendir(path.c_str());
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
std::vector<std::string> get_dir_list_file(std::string path_dir){
  std::vector<std::string> path_vec;
  //---------------------------

  for(const auto& entry : std::experimental::filesystem::directory_iterator(path_dir)){
    path_vec.push_back(entry.path());
  }

  //---------------------------
  return path_vec;
}

// Watcher function
void watcher_all_directory(std::string path){
  std::string event_str;
  //---------------------------

  int EVENT_SIZE = sizeof (struct inotify_event);
  int BUF_LEN = 1024 * ( EVENT_SIZE + 16 );

  int fd = inotify_init();
  int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);

  char buffer[BUF_LEN];
  int length = read( fd, buffer, BUF_LEN );

  struct inotify_event *event = ( struct inotify_event * ) &buffer[0];

  int i = 0;
  while ( i < length ) {
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len ) {
      if ( event->mask & IN_CREATE ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (std::string)event->name + " was created.";
          printf( "The directory %s was created.\n", event->name );
        }
        else {
          event_str = "The file " + (std::string)event->name + " was created.";
          printf( "The file %s was created.\n", event->name );
        }
      }
      else if ( event->mask & IN_DELETE ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (std::string)event->name + " was deleted.";
          printf( "The directory %s was deleted.\n", event->name );
        }
        else {
          event_str = "The file " + (std::string)event->name + " was deleted.";
          printf( "The file %s was deleted.\n", event->name );
        }
      }
      else if ( event->mask & IN_MODIFY ) {
        if ( event->mask & IN_ISDIR ) {
          event_str = "The directory " + (std::string)event->name + " was modified.";
          printf( "The directory %s was modified.\n", event->name );
        }
        else {
          event_str = "The file " + (std::string)event->name + " was modified.";
          printf( "The file %s was modified.\n", event->name );
        }
      }
    }
    i += EVENT_SIZE + event->len;
  }

  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  //---------------------------
}
void watcher_created_file(std::string format_in, std::string path, std::string& path_out, bool& flag){
  std::string event_str;
  //---------------------------

  int EVENT_SIZE = sizeof (struct inotify_event);
  int BUF_LEN = 1024 * ( EVENT_SIZE + 16 );

  int fd = inotify_init();
  int wd = inotify_add_watch(fd, path.c_str(), IN_CREATE);

  char buffer[BUF_LEN];
  int length = read(fd, buffer, BUF_LEN);

  struct inotify_event *event = ( struct inotify_event * ) &buffer[0];

  int i = 0;
  while(i < length){
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len && event->mask & IN_CREATE ) {

      //Full file path
      std::string path_full = path + event->name;
      std::string format = path_full.substr(path_full.find_last_of("."), std::string::npos);

      if(format == format_in){
        //Terminal info
        event_str = "The file " + (std::string)event->name + " was created.";
        printf( "The file %s was created.\n", event->name );

        //Output
        path_out = path + event->name;
        flag = true;
      }
    }
    i += EVENT_SIZE + event->len;
  }

  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  //---------------------------
}
void watcher_modify_file(std::string format_in, std::string path, std::string& path_out, bool& flag){
  std::string event_str;
  //---------------------------

  int EVENT_SIZE = sizeof (struct inotify_event);
  int BUF_LEN = 1024 * ( EVENT_SIZE + 16 );

  int fd = inotify_init();
  int wd = inotify_add_watch(fd, path.c_str(), IN_MODIFY);

  char buffer[BUF_LEN];
  int length = read(fd, buffer, BUF_LEN);

  struct inotify_event *event = ( struct inotify_event * ) &buffer[0];

  int i = 0;
  while(i < length){
    struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
    if ( event->len && event->mask & IN_MODIFY ) {

      //Full file path
      std::string path_full = path + event->name;
      std::string format = path_full.substr(path_full.find_last_of("."), std::string::npos);

      if(format == format_in){
        //Terminal info
        event_str = "The file " + (std::string)event->name + " was modified.";
        printf( "The file %s was modified.\n", event->name );

        //Output
        path_out = path + event->name;
        flag = true;
      }

    }
    i += EVENT_SIZE + event->len;
  }

  ( void ) inotify_rm_watch( fd, wd );
  ( void ) close( fd );

  //---------------------------
}
