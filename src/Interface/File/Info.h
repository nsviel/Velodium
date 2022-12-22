#ifndef INFO_H
#define INFO_H

#include <string>
#include <filesystem>
#include <fstream>
#include <experimental/filesystem>


int get_file_nbPoint(std::string pathFile);
float get_file_size(std::string path);

std::string get_name_from_path(std::string path);
std::string get_format_from_path(std::string path);
std::string get_format_from_filename(std::string path);
std::string get_name_and_parent_from_path(std::string path);
std::string get_filename_from_path(std::string path);
std::string get_type_from_path(std::string path);


#endif
