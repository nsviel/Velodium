/*
*  1 packet = 1 data-frame = 1248 total bytes = 1206 used bytes = 1200 data + 4 timestamp + 2 factory bytes
*  1 packet data = 1200 bytes = 12 blocks of 100 bytes = 24 firing sequences of the 16 laser
*  1 block = 2 firing sequences of the 16 lasers = id | azimuth | data
*  1 frame = 360° rotation of the lasers
*  if data size equal 1206, this is a laser fire data, else about 512, it is a position packet or GPS packet
*/

#include "UDP_parser_HDL32.h"

#include "../../../Specific/fct_maths.h"


//Constructor / Destructor
UDP_parser_HDL32::UDP_parser_HDL32(){
  //---------------------------

  this->nb_laser = 32;
  this->nb_sequences = 24;
  this->supress_emptyElements = false;

  //---------------------------
}
UDP_parser_HDL32::~UDP_parser_HDL32(){}

//Main function
udpPacket* UDP_parser_HDL32::parse_UDP_packet(vector<int> packet){
  udpPacket* cloud = new udpPacket();
  //---------------------------

  //Packet timestamp
  packet_ts_us = packet[1203]*256*256*256 + packet[1202]*256*256 + packet[1201]*256 + packet[1200];

  //Chekc if data is laser or position information
  if(packet.size() != 1206){
    return cloud;
  }

  //Parse packet data
  this->parse_packet(packet);
  this->parse_blocks();
  this->parse_azimuth();
  this->parse_coordinates();
  this->parse_timestamp();
  this->final_check(cloud);

  //---------------------------
  return cloud;
}

//Subfunctions
void UDP_parser_HDL32::parse_packet(vector<int> packet){
  blocks.clear();
  //---------------------------

  //to check if dual return mode / vlp16 data source
  int packet_factory = packet[1205]*256 + packet[1204];

  // velodyne has 12 blocks each 100 bytes data
  for(int i=0; i<12; i++){
    vector<int> block_i;

    for(int j=0; j<100; j++){
      block_i.push_back(packet[j + i*100]);
    }

    blocks.push_back(block_i);
  }

  //---------------------------
}
void UDP_parser_HDL32::parse_blocks(){
  packet_A.clear();
  packet_R.clear();
  packet_I.clear();
  //---------------------------

  // iterate through each block
  for(int i=0; i<blocks.size(); i++){
    vector<int> block_i = blocks[i];

    // block structure: id1 | id2 | azimuth1 | azimuth2 | distance + intensity
    int block_flag = block_i[0]*256 + block_i[1];

    // 0xffee is upper block
    if(block_flag != 65518){
      cout << "Problem of block flag" << endl;
    }

    //Get block azimuth
    float block_azimuth = (float)(block_i[2] + block_i[3] * 256) / 100; //  division by 100 to convert uint into float
    packet_A.push_back(block_azimuth);

    // convert the rest into a matrix 32*3
    for(int i=0; i<32; i++){
      vector<float> point;

      for(int j=0; j<3; j++){
        point.push_back(block_i[j + i*3 + 4]);
      }

      float distance = point[0] + point[1] * 256;
      float intensity = point[2];

      packet_R.push_back(distance);
      packet_I.push_back(intensity);
    }
  }

  //---------------------------
}
void UDP_parser_HDL32::parse_azimuth(){
  //---------------------------

  //Get the actual azimuth for each data point
  vector<float> azimuth_points;
  for(int i=0; i<packet_A.size(); i++){

    // Determine the azimuth Gap between data blocks
    float azimuth_gap;
    if(i < 10){
      if(packet_A[i + 1] < packet_A[i]){
        azimuth_gap = packet_A[i + 1] - packet_A[i] + 360;
      }
      else{
        azimuth_gap = packet_A[i + 1] - packet_A[i];
      }
    }

    int k = 0;
    while(k < 32){
      float azimuth_accurate;

      azimuth_accurate = packet_A[i] + (azimuth_gap * k * 1.152) / 46.08;
      if(azimuth_accurate >= 360){
        azimuth_accurate = azimuth_accurate - 360;
      }

      azimuth_points.push_back(azimuth_accurate);

      k = k + 1;
    }
  }

  //---------------------------
  packet_A = azimuth_points;
}
void UDP_parser_HDL32::parse_coordinates(){
  packet_xyz.clear();
  //---------------------------

  // now calculate the cartesian coordinate of each point
  float laser_angles[32] = { -30.67, -9.33, -29.33, -8.00, -28.00, -6.67, -26.67, -5.33, -25.33, -4.00, -24.00, -2.67, -22.67, -1.33, -21.33, 0.00, -20.00, 1.33, -18.67, 2.67, -17.33, 4.00, -16.00, 5.33, -14.67, 6.67, -13.33, 8.00, -12.00, 9.33, -10.67, 10.67 };
  //float laser_height[32] = { 11.2, -0.7, 9.7, -2.2, 8.1, -3.7, 6.6, -5.1, 5.1, -6.6, 3.7, -8.1, 2.2, -9.7, 0.7, -11.2 }; // in mm
  float FACTOR_CM2M = 0.01;  // factor distance centimeter value to meter
  float FACTOR_MM2CM = 0.2;  // factor distance value to cm, each velodyne distance unit is 2 mm

  // convert distance from millimeter to meter
  for(int i=0; i<packet_R.size(); i++){
    packet_R[i] = packet_R[i] * FACTOR_MM2CM * FACTOR_CM2M;
  }

  //Convert azimuth from degree to radian
  vector<float> azimuth;
  for(int i=0; i<packet_A.size(); i++){
    float az = packet_A[i] * M_PI / 180;
    azimuth.push_back(az);
  }

  //Convert elevation from degree to radian
  vector<float> elevation;
  for(int i=0; i<32; i++){
    float value = laser_angles[i] * M_PI / 180;
    elevation.push_back(value);
  }

  //Carthesian coodinates
  int k = 0;
  for(int i=0; i<packet_R.size(); i++){
    vec3 xyz;

    xyz.x = packet_R[i] * cos(elevation[k]) * sin(azimuth[i]);
    xyz.y = packet_R[i] * cos(elevation[k]) * cos(azimuth[i]);
    xyz.z = packet_R[i] * sin(elevation[k]);

    k++;

    if(k == 32){
      k = 0;
    }

    packet_xyz.push_back(xyz);
  }

  // Laser height correction
  /*for(int i=0; i<packet_xyz.size()/16; i++){
    packet_xyz[i*16] - laser_height[i] * FACTOR_MM2CM * FACTOR_CM2M;
  }*/

  //---------------------------
}
void UDP_parser_HDL32::parse_timestamp(){
  packet_t.clear();
  //---------------------------

  //Timestamp
  float packet_ts_s = packet_ts_us / 1000000; //(us to s)
  float packet_ts_min = packet_ts_s / 60;  // (s to min)

  // calculating timestamp [microsec] of each firing
  vector<float> timing_offsets = calc_timing_offsets();
  for(int i=0; i<timing_offsets.size(); i++){
    packet_t.push_back(packet_ts_s + timing_offsets[i] / 1000000);
  }

  //---------------------------
}
void UDP_parser_HDL32::final_check(udpPacket* cloud){
  //---------------------------

  //Supress points when no distance are measured
  if(supress_emptyElements){
    vector<int> idx;
    for(int i=0; i<packet_R.size(); i++){
      if(packet_R[i] == 0){
        idx.push_back(i);
      }
    }

    this->make_supressElements(packet_I, idx);
    this->make_supressElements(packet_A, idx);
    this->make_supressElements(packet_R, idx);
    this->make_supressElements(packet_t, idx);
    this->make_supressElements(packet_xyz, idx);

    if(packet_xyz.size() == 0){
      cout << "No data in the packet" << endl;
    }
  }

  //Reorder points in function of their timestamp
  vector<vec3> xyz_b;
  vector<float> R_b;
  vector<float> I_b;
  vector<float> A_b;
  vector<float> t_b;
  for (auto i: fct_sortByIndexes(packet_A)){
    xyz_b.push_back(packet_xyz[i]);
    t_b.push_back(packet_t[i]);
    R_b.push_back(packet_R[i]);
    A_b.push_back(packet_A[i]);
    I_b.push_back(packet_I[i]);
  }

  //Store data into udp cloud structure
  cloud->xyz = xyz_b;
  cloud->t = t_b;
  cloud->R = R_b;
  cloud->A = A_b;
  cloud->I = I_b;

  //Check data size
  if(packet_xyz.size() != packet_R.size()){
    cout<< "Problem packet size R" << endl;
  }
  if(packet_xyz.size() != packet_A.size()){
    cout<< "Problem packet size A" << endl;
  }
  if(packet_xyz.size() != packet_t.size()){
    cout<< "Problem packet size t" << endl;
  }
  if(packet_xyz.size() != packet_I.size()){
    cout<< "Problem packet size I" << endl;
  }

  //---------------------------
}

//Subsubfunctions
vector<float> UDP_parser_HDL32::calc_timing_offsets(){
    vector<float> timing_offsets;
    //-----------------------

    //constants
    float full_firing_cycle = 46.08;  // μs
    float single_firing = 1.152;  // μs

    //compute timing offsets
    for(int i=0; i<12; i++){
      for(int j=0; j<32; j++){

        float dataBlockIndex = i;
        float dataPointIndex = j;

        float offset = (full_firing_cycle * dataBlockIndex) + (single_firing * dataPointIndex);
        timing_offsets.push_back(offset);
      }
    }

    //-----------------------
    return timing_offsets;
}
void UDP_parser_HDL32::make_supressElements(vector<vec3>& vec, vector<int> idx){
  if(idx.size() == 0)return;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<vec3> vec_b;
  int cpt = 0;

  for(int i=0; i<vec.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      vec_b.push_back(vec[i]);;
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //---------------------------
  vec = vec_b;
}
void UDP_parser_HDL32::make_supressElements(vector<float>& vec, vector<int> idx){
  if(idx.size() == 0)return;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<float> vec_b;
  int cpt = 0;

  for(int i=0; i<vec.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      vec_b.push_back(vec[i]);
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //---------------------------
  vec = vec_b;
}
