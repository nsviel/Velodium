//Input : CBOR packets
//Output : Subset pointer
//https://github.com/nlohmann/json#binary-formats-cbor-and-messagepack

#include "Scala.h"

#include "../../../Node_interface.h"

#include "../../../../Engine/Node_engine.h"
#include "../../../../Engine/Scene/Configuration.h"
#include "../../../../Load/Node_load.h"
#include "../../../../Load/Processing/Extractor.h"
#include "../../../../Specific/fct_system.h"
#include "../../../../Specific/fct_watcher.h"

#include "../../../../../extern/cbor/json.hpp"

#include <fstream>


//Constructor / Destructor
Scala::Scala(Node_interface* node_interface){
  //---------------------------

  Node_engine* node_engine = node_interface->get_node_engine();
  Node_load* node_load = node_interface->get_node_load();
  Configuration* configManager = node_engine->get_configManager();
  string path_data = configManager->parse_json_s("parameter", "path_data");

  this->extractManager = node_load->get_extractManager();
  this->path_scala = path_data + "scala/";
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
