// (!) not more than one curl command per second
// so just one command per function
//Input : udp packets
//Output : Subset pointer

#include "Velodyne.h"

#include "UDP/UDP_frame.h"
#include "UDP/UDP_server.h"
#include "UDP/UDP_parser_VLP16.h"

#include "../../../Load/Processing/dataExtraction.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>

#include <unistd.h>
#include <fstream>
#include <curl/curl.h>


//Constructor / Destructor
Velodyne::Velodyne(){
  //---------------------------

  this->extractManager = new dataExtraction();

  this->udpServManager = new UDP_server();
  this->udp_vlp16Manager = new UDP_parser_VLP16();
  this->frameManager = new UDP_frame();

  this->ID_subset = 0;
  this->rot_freq = 0;
  this->rot_rpm = 0;
  this->fov_min = 0;
  this->fov_max = 359;

  this->is_first_run = true;
  this->is_capturing = false;
  this->is_rotating = false;
  this->is_connected = false;
  this->is_newSubset = false;

  //---------------------------
}
Velodyne::~Velodyne(){}

//Capturing functions
void Velodyne::lidar_start_watcher(){
  //---------------------------

  //Start udp packets watcher
  thread_capture = std::thread([&]() {
    while (is_capturing) {
      //Get packet in decimal format
      vector<int> packet = udpServManager->read_UDP_packets();

      //Parse decimal packet into point cloud
      udpPacket* packet_parsed = udp_vlp16Manager->parse_UDP_packet(packet);

      //Iteratively build a complete frame
      bool frame_rev = frameManager->build_frame(packet_parsed);

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
void Velodyne::lidar_create_subset(udpPacket* udp_packet){
  //Asynchroneous function (used by theaded watcher)
  //---------------------------

  //Free the memory to get synchroneous data
  udpPacket upd_frame = *udp_packet;
  upd_frame.name = "frame_" + to_string(ID_subset);

  //Convert the udppacket into subset
  this->subset_capture = extractManager->extractData(&upd_frame, 0);

  //Update flags
  if(subset_capture->xyz.size() != 0){
    this->is_newSubset = true;
    this->ID_subset++;
  }

  //---------------------------
}

//LiDAR motor
void Velodyne::lidar_start_motor(){
  //---------------------------

  //If LiDAR not running, start it
  if(rot_freq <= 0){
    int err = system("curl -s --connect-timeout 1 --data rpm=600 http://192.168.1.201/cgi/setting");
    sleep(1);

    rot_freq = 10;
    rot_rpm = 600;

    string log = "LiDAR activated at " + to_string(rot_rpm) + " rpm";
    console.AddLog("#", log);
  }
  //If LiDAR running display the state
  else{
    string log = "LiDAR running at " + to_string(rot_rpm) + " rpm";
    console.AddLog("#", log);
  }

  //---------------------------
  is_rotating = true;
}
void Velodyne::lidar_stop_motor(){
  //---------------------------

  if(rot_freq > 0){
    int err = system("curl -s --connect-timeout 1 --data rpm=0 http://192.168.1.201/cgi/setting");
    rot_freq = 0;
    rot_rpm = 0;

    console.AddLog("#", "LiDAR desactivated");

    sleep(1);
  }

  //---------------------------
  is_rotating = false;
  is_capturing = false;
}

//LiDAR status
void Velodyne::lidar_check_status(){
  if(is_first_run == true){
    //---------------------------

    this->lidar_check_connection();

    if(is_connected){
      //Download a snapshop statut
      int err = system("curl -s --connect-timeout 1 http://192.168.1.201/cgi/status.json > snapshot.hdl");

      std::ifstream ifs("snapshot.hdl");
      Json::Reader reader;
      Json::Value root;

      reader.parse(ifs, root);

      const Json::Value& motor = root["motor"];
      const Json::Value& motor_rpm = motor["rpm"];
      const Json::Value& motor_state = motor["state"];

      const Json::Value& laser = root["laser"];
      const Json::Value& laser_state = laser["state"];

      rot_rpm = motor_rpm.asUInt();
      rot_freq = rot_rpm / 60;

      string log_sta = "Motor state: " + motor_state.asString();
      string log_rpm = "Motor RPM: " + to_string(motor_rpm.asUInt()) + " rpm";
      string log_las = "Laser state: " + laser_state.asString();

      console.AddLog("#", log_sta);
      console.AddLog("#", log_rpm);
      console.AddLog("#", log_las);

      if(rot_freq <= 0){
        is_rotating = false;
      }else{
        is_rotating = true;
      }

      is_first_run = false;
      sleep(1);
    }

    //---------------------------
  }
}
void Velodyne::lidar_check_connection(){
  //---------------------------

  int err = system("curl -s --connect-timeout 1 http://192.168.1.201/");

  if(err == 7168){
    console.AddLog("error", "LiDAR not connected");
    this->is_connected = false;
  }else{
    this->is_connected = true;
  }

  //---------------------------
  sleep(1);
}

//LiDAR parametrization
void Velodyne::lidar_set_rpm(int value){
  //---------------------------

  if(value % 60 != 0){
    cout << "The selected rpm is not modulo 60" << endl;
    return;
  }
  this->rot_rpm = value;

  string rpm = to_string(rot_rpm);
  string command = "curl -s --connect-timeout 1 --data rpm=" + rpm + " http://192.168.1.201/cgi/setting";
  int err = system(command.c_str());

  //---------------------------
  sleep(1);
}
void Velodyne::lidar_set_cameraFOV_min(int value){
  this->fov_min = value;
  //---------------------------

  string command = "curl -s --connect-timeout 1 --data start=" + to_string(fov_min) + " http://192.168.1.201/cgi/setting/fov";
  int err = system(command.c_str());

  //---------------------------
  sleep(1);
}
void Velodyne::lidar_set_cameraFOV_max(int value){
  this->fov_max = value;
  //---------------------------

  string command = "curl -s --connect-timeout 1 --data end=" + to_string(fov_max) + " http://192.168.1.201/cgi/setting/fov";
  int err = system(command.c_str());

  //---------------------------
  sleep(1);
}
void Velodyne::lidar_set_cameraFOV(int min, int max){
  this->fov_min = min;
  this->fov_max = max;
  //---------------------------

  string command = "curl -s --connect-timeout 1 --data start=" + to_string(fov_min) + " --data end=" + to_string(fov_max) + " http://192.168.1.201/cgi/setting/fov";
  int err = system(command.c_str());

  //---------------------------
  sleep(1);
}
