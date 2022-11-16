#include "GUI_Initialization.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Module/Node_module.h"


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();
  this->pathManager = node_load->get_patherManager();

  this->remove_cloud = true;
  this->lidar_model = "velodyne_vlp16";

  //---------------------------
}
GUI_Initialization::~GUI_Initialization(){}

//Main functions
void GUI_Initialization::init_gui(){
  //---------------------------

  //Options
  ImGui::Checkbox("Remove clouds", &remove_cloud);
  static int lidar_model_id = 0;
  ImGui::SetNextItemWidth(125);
  if(ImGui::Combo("Lidar", &lidar_model_id, "velodyne_vlp16\0velodyne_hdl32\0")){
    if(lidar_model_id == 0){
      this->lidar_model = "velodyne_vlp16";
    }else if(lidar_model_id == 1){
      this->lidar_model = "velodyne_hdl32";
    }
  }

  if(ImGui::Button("Buddha", ImVec2(100,0))){
    this->init_mode(0);
  }
  if(ImGui::Button("Torus", ImVec2(100,0))){
    this->init_mode(1);
  }
  if(ImGui::Button("VLP16 - PCAP file", ImVec2(100,0))){
    this->init_mode(2);
  }
  if(ImGui::Button("VLP16 - Nuscene", ImVec2(100,0))){
    this->init_mode(3);
  }
  if(ImGui::Button("VLP64 - Kitti", ImVec2(100,0))){
    this->init_mode(4);
  }
  if(ImGui::Button("HDL32 - Tunel", ImVec2(100,0))){
    this->init_mode(5);
  }
  if(ImGui::Button("VLP16 - Amphitheatre", ImVec2(100,0))){
    this->init_mode(6);
  }

  //---------------------------
}
void GUI_Initialization::init_mode(int mode){
  char path[PATH_MAX];
  //---------------------------

  if(remove_cloud){
    sceneManager->remove_cloud_all();
  }

  switch(mode){
    case 0:{//Buddha
      Matrix4f realTransformation;
      realTransformation <<
        0.306093,   -0.951146,  -0.0403608,    0.585185,
        0.951862,      0.3065, -0.00415026 ,   0.289215,
        0.016318,  -0.0371476 ,   0.999177,   0.0189446,
        0,           0,           0,           1;

      loaderManager->load_cloud("../media/engine/fastScene/buddha.pts");
      loaderManager->load_cloud("../media/engine/fastScene/buddha_moved.pts");
      break;
    }
    case 1:{//Torus
      loaderManager->load_cloud("../media/engine/fastScene/torus_1.ply");
      loaderManager->load_cloud("../media/engine/fastScene/torus_2.ply");
      break;
    }
    case 2:{//VLP16 PCAP
      loaderManager->load_cloud("../media/engine/fastScene/pcap_test.pcap");
      break;
    }
    case 3:{//VLP16 Nuscene
      pathManager->loading_directory_frame("/home/aeter/Desktop/Point_cloud/dataset/NuScene/scene-0002/");
      break;
    }
    case 4:{//VLP16 kitti
      pathManager->loading_directory_frame("../media/point_cloud/kitti/");
      break;
    }
    case 5:{//HDL32 Tunnel
      loaderManager->load_cloud("/home/aeter/Desktop/Point_cloud/pcap/HDL32/HDL32-V2_Tunnel.pcap");
      break;
    }
    case 6:{//VLP16 amphitheatre
      pathManager->loading_directory_frame("/home/aeter/Desktop/Point_cloud/amphitheatre/13.05_amphi_entier/");
      break;
    }
  }

  Cloud* cloud = loaderManager->get_createdcloud();
  cloud->lidar_model = lidar_model;

  //---------------------------
}
