#ifndef WALLET_STRUCT_H
#define WALLET_STRUCT_H

#include <string>
#include <vector>
#include <map>


struct Wallet{
  //---------------------------

  std::map<std::string, std::string> wallet_dic;
  std::vector<std::string> wallet_name;
  std::vector<std::string> wallet_ip;

  std::string selected_name;
  std::string selected_ip;

  //---------------------------
};


#endif
