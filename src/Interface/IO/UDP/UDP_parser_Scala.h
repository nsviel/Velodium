#ifndef UDP_PARSER_SCALA_H
#define UDP_PARSER_SCALA_H

// Server side implementation of UDP client-server model

#include "../../../Engine/Data/struct_UDPpacket.h"
#include "../../../common.h"


class UDP_parser_Scala
{
public:
  //Constructor / Destructor
  UDP_parser_Scala();
  ~UDP_parser_Scala();

public:
  //Main functions
  udpPacket* parse_UDP_packet(vector<int> packet);

  //Subfunctions
  void parse_packet(vector<int> packet);
  void parse_blocks();
  void parse_azimuth();
  void parse_coordinates();
  void parse_timestamp();
  void final_check(udpPacket* cloud);

  //Subsubfunctions
  vector<float> calc_timing_offsets();
  void make_supressElements(vector<vec3>& vec, vector<int> idx);
  void make_supressElements(vector<float>& vec, vector<int> idx);

private:
  vector<vector<int>> blocks;
  vector<vec3> packet_xyz;
  vector<float> packet_R;
  vector<float> packet_I;
  vector<float> packet_A;
  vector<float> packet_t;

  int nb_laser;
  int nb_sequences;
  float packet_ts_us;
  bool supress_emptyElements;
};

#endif
