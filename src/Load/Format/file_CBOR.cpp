#include "file_CBOR.h"

#include "../../../extern/cbor/json.hpp"

#include <fstream>

using cbor = nlohmann::json;


//Constructor / Destructor
file_CBOR::file_CBOR(){}
file_CBOR::~file_CBOR(){}

//Main function
vector<dataFile*> file_CBOR::Loader(string path){
  vector<dataFile*> cloud;
  //---------------------------

  //std::ifstream file(path, std::ios::binary);
  //vector<std::uint8_t> v = {0x42, 0xCA, 0xFE};
  //cbor json = cbor::from_cbor(file);
  //sayHello();
  //ay(json);


  /*std::ifstream file(path, std::ios::binary);
  vector<std::uint8_t> data = readFile(path.c_str());
  cbor json = cbor::from_cbor(file);
  sayHello();
  say(json.is_binary());
  say(json);*/

  vector<std::uint8_t> data = readFile(path.c_str());

  //std::ifstream file(path, std::ios::binary);
  cbor dat_a = cbor::from_cbor(data.data(), data.data() + data.size() );


  //---------------------------
  return cloud;
}

vector<std::uint8_t> file_CBOR::readFile(const char* filename){

            std::ifstream stream(filename, std::ios::in | std::ios::binary);
          std::vector<uint8_t> contents((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

          std::cout << "file size: " << contents.size() << std::endl;

  return contents;
}
