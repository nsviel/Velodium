#include "GUI_Initialization.h"

#include "../GUI_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Load_node.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Module/Module_node.h"
#include "../../Module/Player/Player_node.h"
#include "../../Module/Player/Obstacle/Obstacle.h"
#include "../../Module/SLAM/Slam.h"
#include "../../Module/SLAM/CT_ICP/SLAM_parameter.h"


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(GUI_node* node_gui){
  //---------------------------

  Engine_node* node_engine = node_gui->get_node_engine();
  Module_node* node_module = node_gui->get_node_module();
  Player_node* node_player = node_module->get_node_player();
  Load_node* node_load = node_engine->get_node_load();
  Slam* slamManager = node_module->get_slamManager();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->obstacleManager = node_player->get_obstacleManager();
  this->loaderManager = node_load->get_loadManager();
  this->pathManager = node_load->get_pathManager();
  this->slam_param = slamManager->get_slam_param();

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
  /*if(ImGui::Button("AI interface", ImVec2(100,0))){
    this->init_mode(6);
  }*/

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
      pathManager->loading_directoryFrames("/home/aether/Desktop/Point_cloud/dataset/NuScene/scene-0002/");
      break;
    }
    case 4:{//VLP16 kitti
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp64");
      pathManager->loading_directoryFrames("../media/point_cloud/kitti/");
      break;
    }
    case 5:{//HDL32 Tunnel
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_hdl32");
      loaderManager->load_cloud("/home/aether/Desktop/Point_cloud/pcap/HDL32/HDL32-V2_Tunnel.pcap");
      break;
    }
    case 6:{//AI setup
      sceneManager->remove_cloud_all();
      slam_param->make_config("velodyne_vlp64");
      pathManager->loading_directoryFrames("../media/point_cloud/frames/");
      obstacleManager->add_obstacle_pred("/home/aether/Desktop/Velodium/media/data/capture_test/prediction/");
      configManager->make_preconfig(1);
      break;
    }
  }

  //---------------------------
}
