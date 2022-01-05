#include "file_PCAP.h"

#include "../../Module/Velodyne/UDP/UDP_parser_VLP16.h"
#include "../../Module/Velodyne/UDP/UDP_parser_HDL32.h"
#include "../../Module/Velodyne/UDP/UDP_frame.h"

#include <tins/tins.h>
#include <iostream>
#include <stddef.h>
#include <bitset>

using namespace Tins;
vector<vector<int>> file_packets;
size_t lenght(0);


//Constructor / Destructor
file_PCAP::file_PCAP(){
  //---------------------------

  this->LiDAR_model = "vlp16";
  this->packet_max = 0;
  this->packet_beg = 0;
  this->packet_end = 0;

  //---------------------------
}
file_PCAP::~file_PCAP(){}

//PCAP reader callback
bool parse_packets(const PDU& packet){
  //---------------------------

  //Retrieve data packet
  const RawPDU raw = packet.rfind_pdu<RawPDU>();
  vector<uint8_t> buffer = raw.payload();

  //Convert into decimal vector
  vector<int> packet_dec;
  for(int i =0; i < buffer.size(); i++){
    std::bitset<8> octet(buffer[i]);

    int octet_32 = octet.to_ulong();
    packet_dec.push_back(octet_32);
  }

  //Store the packet
  file_packets.push_back(packet_dec);

  lenght++;
  if(lenght == 100)return false;
  //---------------------------
  return true;
}
bool count_packets(const PDU &){
    lenght++;
    return true;
}

//Main function
vector<dataFile*> file_PCAP::Loader(string pathFile){
  data_vec.clear();
  file_packets.clear();
  //---------------------------

  //Check if vlp16 or hdl32
  if (pathFile.find("HDL32") != string::npos){
    this->LiDAR_model = "hdl32";
  }else{
    this->LiDAR_model = "vlp16";
  }

  //Open file
  FileSniffer sniffer(pathFile);
  sniffer.sniff_loop(parse_packets);
  say(lenght);

  //Parse data
  if(LiDAR_model == "vlp16"){
    this->Loader_vlp16(pathFile);
  }
  else if(LiDAR_model == "hdl32"){
    this->Loader_hdl32(pathFile);
  }

  //---------------------------
  return data_vec;
}
vector<dataFile*> file_PCAP::Loader(string pathFile, int nb_packet){
  data_vec.clear();
  file_packets.clear();
  //---------------------------

  //Check if vlp16 or hdl32
  if (pathFile.find("HDL32") != string::npos){
    this->LiDAR_model = "hdl32";
  }else{
    this->LiDAR_model = "vlp16";
  }

  say(pathFile);

  //Read UDP packets
  vector<Tins::Packet> vt;
  Sniffer sniffer(pathFile);sayHello();
  while (vt.size() != nb_packet) {
    vt.push_back(sniffer.next_packet());
  }

  sayHello();
  sayVec(vt);

  for (const auto& packet : vt) {
    const RawPDU raw = packet.pdu()->rfind_pdu<RawPDU>();
    vector<uint8_t> buffer = raw.payload();

    //Convert into decimal vector
    vector<int> packet_dec;
    for(int i =0; i < buffer.size(); i++){
      std::bitset<8> octet(buffer[i]);

      int octet_32 = octet.to_ulong();
      packet_dec.push_back(octet_32);
    }

    //Store the packet
    file_packets.push_back(packet_dec);
  }

sayHello();
  say(file_packets.size());

  //Parse data
  if(LiDAR_model == "vlp16"){
    this->Loader_vlp16(pathFile);
  }
  else if(LiDAR_model == "hdl32"){
    this->Loader_hdl32(pathFile);
  }

  //---------------------------
  return data_vec;
}

void file_PCAP::Loader_vlp16(string pathFile){
  UDP_frame frameManager;
  UDP_parser_VLP16 udpManager;
  //---------------------------

  int cpt = 0;
  for(int i=0; i<file_packets.size(); i++){
    udpPacket* cloud = udpManager.parse_UDP_packet(file_packets[i]);
    bool frame_rev = frameManager.build_frame(cloud);

    if(frame_rev){
      udpPacket* frame = frameManager.get_endedFrame();
      dataFile* frame_data = new dataFile();

      frame_data->name = "frame_" + to_string(cpt); cpt++;
      frame_data->path = pathFile;
      frame_data->size = frame->xyz.size();

      for(int j=0; j<frame->xyz.size(); j++){
        frame_data->location.push_back(frame->xyz[j]);
        frame_data->intensity.push_back(frame->I[j] / 255);
        frame_data->timestamp.push_back(frame->t[j]);
        frame_data->azimuth.push_back(frame->A[j]);
        frame_data->distance.push_back(frame->R[j]);
      }

      data_vec.push_back(frame_data);
    }
  }

  //---------------------------
}
void file_PCAP::Loader_hdl32(string pathFile){
  UDP_frame frameManager;
  UDP_parser_HDL32 udpManager;
  //---------------------------

  for(int i=0; i<file_packets.size(); i++){
    udpPacket* cloud = udpManager.parse_UDP_packet(file_packets[i]);
    bool frame_rev = frameManager.build_frame(cloud);

    if(frame_rev){
      udpPacket* frame = frameManager.get_endedFrame();
      dataFile* frame_data = new dataFile();

      frame_data->path = pathFile;
      frame_data->size = frame->xyz.size();

      for(int j=0; j<frame->xyz.size(); j++){
        frame_data->location.push_back(frame->xyz[j]);
        frame_data->intensity.push_back(frame->I[j] / 255);
        frame_data->timestamp.push_back(frame->t[j]);
        frame_data->azimuth.push_back(frame->A[j]);
        frame_data->distance.push_back(frame->R[j]);
      }

      data_vec.push_back(frame_data);
    }
  }

  //---------------------------
}
int file_PCAP::get_file_length(string pathFile){
  lenght = 0;
  //---------------------------

  FileSniffer sniffer(pathFile);
  sniffer.sniff_loop(count_packets);

  //---------------------------
  return lenght;
}
