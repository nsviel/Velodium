#ifndef UDP_PARSER_SCALA_H
#define UDP_PARSER_SCALA_H

// Server side implementation of UDP client-server model

#include "../../../../Scene/Base/struct_data_file.h"
#include "../../../../common.h"


class Parser_Scala
{
public:
  //Constructor / Destructor
  Parser_Scala();
  ~Parser_Scala();

public:
  //Main functions
  Data_file* parse_packet(vector<int> packet);

  //Subfunctions
  void parse_vector(vector<int> packet);
  void parse_blocks();
  void parse_azimuth();
  void parse_coordinates();
  void parse_timestamp();
  void final_check(Data_file* cloud);

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
