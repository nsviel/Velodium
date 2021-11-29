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


//Constructor / Destructor
filePCAP::filePCAP(){
  //---------------------------

  this->vlp16 = true;
  this->hdl32 = false;

  //---------------------------
}
filePCAP::~filePCAP(){}

//PCAP reader callback
bool parse_packets(const PDU & packet){
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

  //---------------------------
  return true;
}

//Main function
vector<dataFile*> filePCAP::Loader(string pathFile){
  data_vec.clear();
  file_packets.clear();
  //---------------------------

  //Check if vlp16 or hdl32
  if (pathFile.find("HDL32") != string::npos){
    this->vlp16 = false;
    this->hdl32 = true;
  }

  //Open file
  FileSniffer sniffer(pathFile);
  sniffer.sniff_loop(parse_packets);

  //Parse data
  UDP_frame frameManager;

  if(vlp16){
    int cpt = 0;
    UDP_parser_VLP16 udpManager;
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
  }
  else if(hdl32){
    UDP_parser_HDL32 udpManager;
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
  }

  //---------------------------
  return data_vec;
}
