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


void clean_directory_files(const char *path);
void create_new_dir(std::string path);

bool is_dir(std::string path);
bool is_file_exist(std::string fileName);
bool is_dir_exist(std::string path);

int get_dir_numberOfFile(std::string path);

std::vector<std::string> list_allFiles(std::string path);
std::vector<std::string> list_allPaths(std::string path);
std::vector<std::string> list_allDirs(std::string path);
std::vector<std::string> get_dir_list_file(std::string path_dir);

// Watcher function
void watcher_all_directory(std::string path);
void watcher_created_file(std::string format_in, std::string path, std::string& path_out, bool& flag);
void watcher_modify_file(std::string format_in, std::string path, std::string& path_out, bool& flag);


#endif
