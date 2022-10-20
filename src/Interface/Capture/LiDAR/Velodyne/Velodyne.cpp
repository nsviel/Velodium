// (!) not more than one curl command per second
// so just one command per function
//Input : udp packets
//Output : Subset pointer

#include "Velodyne.h"

#include "../../UDP/UDP_frame.h"
#include "../../UDP/UDP_server.h"
#include "../../UDP/UDP_parser_VLP16.h"

#include "../../../Node_interface.h"

#include "../../../../Engine/Node_engine.h"
#include "../../../../Load/Node_load.h"
#include "../../../../Load/Processing/Extractor.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>

#include <unistd.h>
#include <fstream>
#include <curl/curl.h>


//Constructor / Destructor
Velodyne::Velodyne(Node_interface* node_interface){
  //---------------------------

  Node_load* node_load = node_interface->get_node_load();

  this->extractManager = node_load->get_extractManager();
  this->udpServManager = new UDP_server();
  this->udp_vlp16Manager = new UDP_parser_VLP16();
  this->frameManager = new UDP_frame();
  this->subset_capture = new Subset();

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

Subset* Velodyne::get_subset_capture(){
  //---------------------------

  //Free the memory to get synchroneous data
  udp_capture.name = "";

  //Convert the udp packet into subset
  Subset* subset = extractManager->extract_data(udp_capture);

  //---------------------------
  return subset;
}

//Capturing functions
void Velodyne::lidar_start_watcher(int capture_port){
  this->is_capturing = true;
  //---------------------------

  //Start udp packets watcher
  thread_capture = std::thread([&]() {
    int port = capture_port;
    int size_max = 1248;
    port = 2370;
    udpServManager->capture_init(port, size_max);


    while (is_capturing){
      //Get packet in decimal format
      vector<int> packet_dec = udpServManager->capture_packet();

      //Parse decimal packet into point cloud
      if(packet_dec.size() != 0){
        udpPacket* packet_udp = udp_vlp16Manager->parse_UDP_packet(packet_dec);

        //Iteratively build a complete frame
        bool frame_rev = frameManager->build_frame(packet_udp);

        if(frame_rev){
          udpPacket* frame = frameManager->get_endedFrame();
          this->udp_capture = *frame;

          //Do not record the first frame
          if(is_first_run == false){
            this->is_newSubset = true;
          }else{
            this->is_first_run = false;
          }
        }
      }
    }

    udpServManager->capture_stop();
  });
  thread_capture.detach();

  //---------------------------
}
void Velodyne::lidar_stop_watcher(){
  //---------------------------

  this->is_capturing = false;
  frameManager->reset_frame();

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

  //Send stop motor command
  if(rot_freq > 0){
    int err = system("curl -s --connect-timeout 1 --data rpm=0 http://192.168.1.201/cgi/setting");
    rot_freq = 0;
    rot_rpm = 0;

    console.AddLog("#", "LiDAR desactivated");

    sleep(1);
  }

  //Set state
  this->lidar_stop_watcher();
  this->is_rotating = false;

  //---------------------------
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
