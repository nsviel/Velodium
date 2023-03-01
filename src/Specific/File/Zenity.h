#ifndef ZENITY_H
#define ZENITY_H

#include <string>
#include <vector>


std::vector<std::string> zenity_file_vec(std::string title, std::string& path_dir);
std::vector<std::string> zenity_file_vec(std::string title);
std::string zenity_directory(std::string title, std::string& path_dir);
void zenity_file(std::string& path_file);
std::string zenity_file();
void zenity_directory(std::string& path_dir);
std::string zenity_saving(std::string& dir, std::string filename);


#endif
