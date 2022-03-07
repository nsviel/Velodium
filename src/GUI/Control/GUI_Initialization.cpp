#include "GUI_Initialization.h"

#include "../GUI_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Load/Load_node.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Module/Module_node.h"
#include "../../Module/Player/Player_node.h"
#include "../../Module/Player/Obstacle/Obstacle.h"
#include "../../Module/Player/Obstacle/Scenario.h"


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(GUI_node* node_gui){
  //---------------------------

  Engine_node* node_engine = node_gui->get_node_engine();
  Module_node* node_module = node_gui->get_node_module();
  Player_node* node_player = node_module->get_node_player();
  Load_node* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->obstacleManager = node_player->get_obstacleManager();
  this->scenarnode_interface = node_player->get_scenarnode_interface();
  this->loaderManager = node_load->get_loadManager();
  this->pathManager = node_load->get_pathManager();

  //---------------------------
}
GUI_Initialization::~GUI_Initialization(){}

//Main functions
void GUI_Initialization::init_gui(){
  //---------------------------

  //Two Buddha point cloud to register
  if(ImGui::Button("Buddha", ImVec2(100,0))){
    this->init_mode(0);
  }
  //Two Torus point cloud to register
  if(ImGui::Button("Torus", ImVec2(100,0))){
    this->init_mode(1);
  }
  if(ImGui::Button("PCAP file", ImVec2(100,0))){
    this->init_mode(2);
  }
  if(ImGui::Button("Frames move", ImVec2(100,0))){
    this->init_mode(3);
  }
  if(ImGui::Button("More frames", ImVec2(100,0))){
    this->init_mode(4);
  }
  if(ImGui::Button("Other frames", ImVec2(100,0))){
    this->init_mode(5);
  }
  if(ImGui::Button("Tunel", ImVec2(100,0))){
    this->init_mode(6);
  }
  if(ImGui::Button("IA module", ImVec2(100,0))){
    this->init_mode(7);
  }
  if(ImGui::Button("Velodyne statique", ImVec2(100,0))){
    this->init_mode(8);
  }
  if(ImGui::Button("Velodyne move", ImVec2(100,0))){
    this->init_mode(9);
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
    case 2:{//PCAP
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("../media/engine/fastScene/pcap_test.pcap");
      break;
    }
    case 3:{//Frames in movement
      sceneManager->remove_cloud_all();
      pathManager->loading_directoryFrames("../media/point_cloud/frames/");
      break;
    }
    case 4:{//more Frames in movement
      sceneManager->remove_cloud_all();
      pathManager->loading_directoryFrames("../media/point_cloud/frames_lot/");
      break;
    }
    case 5:{//Frame other
      sceneManager->remove_cloud_all();
      pathManager->loading_directoryFrames("../media/point_cloud/frames_other/");
      break;
    }
    case 6:{//Frame other
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("/home/aether/Desktop/Point_cloud/HDL32-V2_Tunnel.pcap");
      break;
    }
    case 7:{//AI prediction
      sceneManager->remove_cloud_all();
      pathManager->loading_directoryFrames("../media/point_cloud/frames/");
      obstacleManager->add_prediction("/home/aether/Desktop/Velodium/media/data/capture_test/prediction/");
      scenarnode_interface->make_configuration();
      break;
    }
    case 8:{//Frame for prediction
      sceneManager->remove_cloud_all();
      pathManager->loading_directoryFrames("/home/aether/Desktop/Velodium/media/data/capture/test_statique");
      break;
    }
    case 9:{//Frame for prediction

      break;
    }
  }

  //---------------------------
}
