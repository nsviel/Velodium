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

  //Concatenate it into combo format
  void make_concatenation(){
    this->wallet_name.clear();
    this->wallet_ip.clear();
    for (auto const& [key, val] : wallet_dic){
      this->wallet_name.push_back(key);
      this->wallet_ip.push_back(val);
    }
  }

  //---------------------------
};


#endif
