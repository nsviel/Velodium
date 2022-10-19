#include "GUI_Initialization.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Module/Node_module.h"
#include "../../Module/SLAM/Module_slam.h"
#include "../../Module/SLAM/src/SLAM_parameter.h"
#include "../../Operation/Node_operation.h"


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_operation* node_ope = node_engine->get_node_ope();
  Node_load* node_load = node_engine->get_node_load();
  Node_module* node_module = node_engine->get_node_module();
  Module_slam* module_slam = node_module->get_module_slam();

  this->slam_param = module_slam->get_slam_param();
  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->pathManager = node_load->get_pathManager();

  //---------------------------
}
GUI_Initialization::~GUI_Initialization(){}

//Main functions
void GUI_Initialization::init_gui(){
  //---------------------------

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

  switch(mode){
    case 0:{//Buddha
      sceneManager->remove_cloud_all();

      Matrix4f realTransformation;
      realTransformation <<
        0.306093,   -0.951146,  -0.0403608,    0.585185,
        0.951862,      0.3065, -0.00415026 ,   0.289215,
        0.016318,  -0.0371476 ,   0.999177,   0.0189446,
        0,           0,           0,           1;

      loaderManager->load_cloud("../media/engine/fastScene/buddha.pts");
      loaderManager->load_cloud("../media/engine/fastScene/buddha_moved.pts");
      //loaderManager->load_cloud("../media/engine/fastScene/buddha_moved.pts", realTransformation);
      break;
    }
    case 1:{//Torus
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("../media/engine/fastScene/torus_1.ply");
      loaderManager->load_cloud("../media/engine/fastScene/torus_2.ply");
      break;
    }
    case 2:{//VLP16 PCAP
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp16");
      loaderManager->load_cloud("../media/engine/fastScene/pcap_test.pcap");
      break;
    }
    case 3:{//VLP16 Nuscene
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp16");
      pathManager->loading_directory_frame("/home/aether/Desktop/Point_cloud/dataset/NuScene/scene-0002/");
      break;
    }
    case 4:{//VLP16 kitti
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp64");
      pathManager->loading_directory_frame("../media/point_cloud/kitti/");
      break;
    }
    case 5:{//HDL32 Tunnel
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_hdl32");
      loaderManager->load_cloud("/home/aether/Desktop/Point_cloud/pcap/HDL32/HDL32-V2_Tunnel.pcap");
      break;
    }
    case 6:{//VLP16 amphitheatre
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp16");
      pathManager->loading_directory_frame("/home/aether/Desktop/Point_cloud/13.05_amphi_entier/");
      break;
    }
  }

  //---------------------------
}
