//Input : udp packets
//Output : Subset pointer

#include "Scala.h"

#include "../../../../Load/UDP/UDP_frame.h"
#include "../../../../Load/UDP/UDP_server.h"
#include "../../../../Load/UDP/UDP_parser_Scala.h"

#include "../../../../Engine/Engine_node.h"
#include "../../../../Load/Load_node.h"
#include "../../../../Load/Processing/Extractore.h"

#include "../../../../../extern/json.hpp"


//Constructor / Destructor
Scala::Scala(Engine_node* node_engine){
  //---------------------------

  Load_node* node_load = node_engine->get_node_load();

  this->extractManager = node_load->get_extractManager();
  this->udpServManager = new UDP_server();
  this->udp_scalaManager = new UDP_parser_Scala();
  this->frameManager = new UDP_frame();
  this->subset_capture = new Subset();

  this->is_first_run = true;
  this->is_capturing = false;
  this->is_newSubset = false;

  //---------------------------
}
Scala::~Scala(){}

//Capturing functions
void Scala::lidar_start_watcher(){
  //---------------------------

  //Start udp packets watcher
  thread_capture = std::thread([&]() {
    while (is_capturing){
      //Get packet in decimal format
      vector<int> packet_dec = udpServManager->read_UDP_packets();

      //Parse decimal packet into point cloud
      udpPacket* packet_udp = udp_scalaManager->parse_UDP_packet(packet_dec);

      //Iteratively build a complete frame
      bool frame_rev = frameManager->build_frame(packet_udp);

      if(frame_rev){
        udpPacket* frame = frameManager->get_endedFrame();
        this->lidar_create_subset(frame);
      }
    }
  });
  thread_capture.detach();

  //---------------------------
  this->is_capturing = true;
}
void Scala::lidar_create_subset(udpPacket* udp_packet){
  //Asynchroneous function (used by theaded watcher)
  //---------------------------

  //Free the memory to get synchroneous data
  udpPacket upd_frame = *udp_packet;
  upd_frame.name = "";

  //Convert the udp packet into subset
  Subset* subset_extracted = extractManager->extractData(upd_frame);

  //Make sure to desallocate memory
  delete subset_capture;
  this->subset_capture = new Subset(*subset_extracted);
  delete subset_extracted;

  //Update flags
  if(subset_capture->xyz.size() != 0){
    this->is_newSubset = true;
  }

  //---------------------------
}
