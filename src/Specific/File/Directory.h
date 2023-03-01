#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <string>
#include <vector>
#include <cstring>
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

#include "../../common.h"


//Directory operations
void dir_clean_file(const char *path);
void dir_create_new(std::string path);
int dir_number_file(std::string path);

//Check existence
bool is_file_exist(std::string fileName);
bool is_dir_exist(std::string path);
std::string is_dir_or_file(std::string path);

//List files & paths
std::vector<std::string> list_all_file(std::string path);
std::vector<std::string> list_all_path(std::string path);
std::vector<std::string> list_all_dir(std::string path);

// Watcher function
void watcher_all_directory(std::string path);
void watcher_created_file(std::string format_in, std::string path, std::string& path_out, bool& flag);
void watcher_modify_file(std::string format_in, std::string path, std::string& path_out, bool& flag);


#endif
