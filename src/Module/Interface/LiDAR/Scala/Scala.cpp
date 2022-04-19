//Input : CBOR packets
//Output : Subset pointer
//https://github.com/nlohmann/json#binary-formats-cbor-and-messagepack

#include "Scala.h"

#include "../../Local/Filemanager.h"
#include "../../Interface_node.h"

#include "../../../../Load/UDP/UDP_frame.h"
#include "../../../../Load/UDP/UDP_server.h"
#include "../../../../Load/UDP/UDP_parser_Scala.h"

#include "../../../../Engine/Engine_node.h"
#include "../../../../Load/Load_node.h"
#include "../../../../Load/Processing/Extractor.h"
#include "../../../../Specific/fct_system.h"
#include "../../../../Specific/fct_watcher.h"

#include "../../../../../extern/json.hpp"

#include <fstream>


//Constructor / Destructor
Scala::Scala(Interface_node* node){
  //---------------------------

  Filemanager* fileManager = node->get_fileManager();
  Engine_node* node_engine = node->get_node_engine();
  Load_node* node_load = node_engine->get_node_load();

  this->extractManager = node_load->get_extractManager();
  this->path_scala = fileManager->get_path_data_dir() + "scala/";
  this->is_capturing = false;
  this->is_new_subset = false;
  this->capture_port = 2371;

  //---------------------------
}
Scala::~Scala(){}

//Capturing functions
void Scala::lidar_start_watcher(){
  //---------------------------

  //Start udp packets watcher
  thread_scala = std::thread([&]() {
    while (is_capturing){
      //Watch for new file
      watcher_created_file(".cbor", path_scala, path_scala_file, flag_newScala);

      // read a JSON file
      std::ifstream file(path_scala_file);
      nlohmann::json json_file;
      file >> json_file;
    }
  });
  thread_scala.detach();

  //---------------------------
  this->is_capturing = true;
}
void Scala::lidar_create_subset(udpPacket* udp_packet){
  //Asynchroneous function (used by theaded watcher)
  //---------------------------


  //---------------------------
}
