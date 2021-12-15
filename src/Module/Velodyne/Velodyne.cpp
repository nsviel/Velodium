// (!) not more than one curl command per second
// so just one command per function

#include "Velodyne.h"

#include "UDP/UDP_frame.h"
#include "UDP/UDP_server.h"
#include "UDP/UDP_parser_VLP16.h"

#include "../../Engine/Scene.h"
#include "../../Load/Loader.h"
#include "../../Load/dataExtraction.h"
#include "../../Specific/timer.h"

#include <jsoncpp/json/value.h>
#include <jsoncpp/json/json.h>

#include <unistd.h>
#include <fstream>
#include <curl/curl.h>
#include <experimental/filesystem>


//Constructor / Destructor
Velodyne::Velodyne(){
  //---------------------------

  this->udpServManager = new UDP_server();
  this->udpParsManager = new UDP_parser_VLP16();
  this->frameManager = new UDP_frame();
  this->loaderManager = new Loader();
  this->extractManager = new dataExtraction();
  this->timerManager = new Timer();

  this->rot_freq = 0;
  this->fov_min = 0;
  this->fov_max = 359;
  this->record_n_frame_max = 10;
  this->is_record_t_frame_max = 10;

  this->has_started = false;
  this->is_first_run = true;
  this->is_capturing = false;
  this->is_rotating = false;
  this->is_connected = false;
  this->is_recording = false;

  //Get absolute executable location
  string absPath = std::experimental::filesystem::current_path();
  this->saveas = absPath + "/../media/data/";

  //---------------------------
}
Velodyne::~Velodyne(){}

//Recording functions
void Velodyne::run_capture(){
  atleastoneframe = false;
  //---------------------------

  m_thread = std::thread([&]() {
    while (is_capturing) {
      //Get packet in decimal format
      vector<int> packet = udpServManager->read_UDP_packets();

      //Parse decimal packet into point cloud
      udpPacket* packet_parsed = udpParsManager->parse_UDP_packet(packet);

      //Iteratively build a complete frame
      bool frame_rev = frameManager->build_frame(packet_parsed);

      if(frame_rev){
        atleastoneframe = true;
        frame = frameManager->get_endedFrame();
      }
    }
  });
  m_thread.detach();

  //---------------------------
}
void Velodyne::onrun_ope(){
  //---------------------------

  if(atleastoneframe){
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = &cloud->subset[0];
    udpPacket frame_new = *frame;

    subset->xyz.clear();
    subset->I.clear();
    subset->ts.clear();
    subset->A.clear();
    subset->R.clear();

    subset->nb_point = frame_new.xyz.size();
    subset->root = vec3(0.0);

    for(int j=0; j<frame_new.xyz.size(); j++){
      subset->xyz.push_back(frame_new.xyz[j]);
      subset->I.push_back(frame_new.I[j] / 255);
      subset->ts.push_back(frame_new.t[j]);
      subset->A.push_back(frame_new.A[j]);
      subset->R.push_back(frame_new.R[j]);

      float rgb = frame_new.I[j] / 255;
      subset->RGB.push_back(vec4(rgb,rgb,rgb,1.0f));
    }

    //Update list cloud
    Scene sceneManager;
    sceneManager.update_subset_IntensityToColor(subset);
    sceneManager.update_subset_location(subset);

    if(is_recording){
      string fileName = subset->name + "_" + to_string(subset_selected);
      loaderManager->save_subset(subset, "ply", saveas, fileName);
      this->subset_selected++;
      this->record_n_frame_nb++;

      //Option: record n frames
      if(is_record_n_frame){
        if(record_n_frame_nb >= record_n_frame_max){
          is_recording = false;
        }

      }
    }
  }

  //---------------------------
}
void Velodyne::recording_selectDirSave(){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + saveas;

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
    }

    this->saveas = path_str + "/";
  }

  //---------------------------
}

//LiDAR functions
void Velodyne::lidar_start(){
  //---------------------------

  if(lidar_get_is_connected() == false){
    return;
  }

  if(is_first_run == true){
    this->lidar_get_status();
    is_first_run = false;
  }

  //If LiDAR not running
  if(rot_freq <= 0){
    int err = system("curl -s --connect-timeout 1 --data rpm=600 http://192.168.1.201/cgi/setting");
    sleep(1);

    rot_freq = 10;
    rot_rpm = 600;

    is_rotating = true;

    string log = "LiDAR activated at " + to_string(rot_rpm) + " rpm";
    console.AddLog("#", log);
  }
  //If LiDAR running display the state
  else{
    string log = "LiDAR running at " + to_string(rot_rpm) + " rpm";
    console.AddLog("#", log);
  }

  //---------------------------
}
void Velodyne::lidar_stop(){
  //---------------------------

  if(rot_freq > 0){
    int err = system("curl -s --connect-timeout 1 --data rpm=0 http://192.168.1.201/cgi/setting");
    rot_freq = 0;
    rot_rpm = 0;

    is_rotating = false;
    is_capturing = false;

    console.AddLog("#", "LiDAR desactivated");

    sleep(1);
  }

  //---------------------------
}
void Velodyne::lidar_startNewCapture(){
  //---------------------------

  this->record_n_frame_nb = 0;
  this->subset_selected = 0;
  loaderManager->load_cloud_empty();
  Cloud* cloud = sceneManager->get_cloud_selected();
  int ID = *sceneManager->get_list_ID_cloud();
  cloud->name = "capture_" + to_string(ID);

  //Set timer
  if(timerManager->isRunning()){
    timerManager->stop();
    record_t_frame_sec = 0;
  }
  timerManager->setFunc([&](){
		record_t_frame_sec++;
		cout << record_t_frame_sec << endl;
	});
  timerManager->setInterval(1000);
  timerManager->start();

  //---------------------------
}
void Velodyne::lidar_get_status(){
  //---------------------------

  //Download a snapshop statut
  int err = system("curl -s --connect-timeout 1 http://192.168.1.201/cgi/status.json > snapshot.hdl");

  std::ifstream ifs("snapshot.hdl");
  Json::Reader reader;
  Json::Value obj;

  reader.parse(ifs, obj);

  const Json::Value& motor = obj["motor"];
  const Json::Value& motor_rpm = motor["rpm"];
  const Json::Value& motor_state = motor["state"];

  const Json::Value& laser = obj["laser"];
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

  //---------------------------
  sleep(1);
}
bool Velodyne::lidar_get_is_connected(){
  bool connected;
  //---------------------------

  int err = system("curl -s --connect-timeout 1 http://192.168.1.201/");

  if(err == 7168){
    console.AddLog("error", "LiDAR not connected");
    this->is_connected = false;
    connected = false;
  }else{
    this->is_connected = true;
    connected = true;
  }

  //---------------------------
  sleep(1);
  return connected;
}
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
