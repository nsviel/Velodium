#include "GUI_Initialization.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Operation/Transformation/Transformation.h"
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
  this->cloud_scale = 1;
  this->lidar_model = "velodyne_vlp16";

  //---------------------------
}
GUI_Initialization::~GUI_Initialization(){}

//Main functions
void GUI_Initialization::init_gui(){
  //---------------------------

  //Options
  this->operation_option();

  if(ImGui::Button("Buddha", ImVec2(100,0))){
    this->operation_init(0);
  }
  if(ImGui::Button("Torus", ImVec2(100,0))){
    this->operation_init(1);
  }
  if(ImGui::Button("Bunny", ImVec2(100,0))){
    this->operation_init(7);
  }
  if(ImGui::Button("VLP16 - PCAP file", ImVec2(100,0))){
    this->lidar_model = "velodyne_vlp16";
    this->operation_init(2);
  }
  if(ImGui::Button("VLP16 - Nuscene", ImVec2(100,0))){
    this->lidar_model = "velodyne_vlp16";
    this->operation_init(3);
  }
  if(ImGui::Button("VLP64 - Kitti", ImVec2(100,0))){
    this->lidar_model = "velodyne_vlp64";
    this->operation_init(4);
  }
  if(ImGui::Button("HDL32 - Tunel", ImVec2(100,0))){
    this->lidar_model = "velodyne_hdl32";
    this->operation_init(5);
  }
  if(ImGui::Button("VLP16 - Amphitheatre", ImVec2(100,0))){
    this->lidar_model = "velodyne_vlp16";
    this->operation_init(6);
  }

  //---------------------------
}
void GUI_Initialization::operation_init(int mode){
  char path[PATH_MAX];
  //---------------------------

  if(remove_cloud){
    sceneManager->remove_cloud_all();
  }

  switch(mode){
    case 0:{//Buddha
      loaderManager->load_cloud("../media/engine/fastScene/buddha.pts");
      this->operation_cloud(loaderManager->get_createdcloud());
      loaderManager->load_cloud("../media/engine/fastScene/buddha_moved.pts");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 1:{//Torus
      loaderManager->load_cloud("../media/engine/fastScene/torus_1.ply");
      this->operation_cloud(loaderManager->get_createdcloud());
      loaderManager->load_cloud("../media/engine/fastScene/torus_2.ply");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 2:{//VLP16 PCAP
      loaderManager->load_cloud("../media/engine/fastScene/pcap_test.pcap");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 3:{//VLP16 Nuscene
      pathManager->loading_directory_frame("/home/aeter/Desktop/Point_cloud/dataset/NuScene/scene-0002/");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 4:{//VLP16 kitti
      pathManager->loading_directory_frame("../media/point_cloud/kitti/");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 5:{//HDL32 Tunnel
      loaderManager->load_cloud("/home/aeter/Desktop/Point_cloud/pcap/HDL32/HDL32-V2_Tunnel.pcap");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 6:{//VLP16 amphitheatre
      pathManager->loading_directory_frame("/home/aeter/Desktop/Point_cloud/amphitheatre/13.05_amphi_entier/");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
    case 7:{//Bunny
      loaderManager->load_cloud("/home/aeter/Desktop/System/Velodium/media/engine/Marks/bunny.ply");
      this->operation_cloud(loaderManager->get_createdcloud());
      break;
    }
  }



  //---------------------------
}
void GUI_Initialization::operation_cloud(Cloud* cloud){
  //---------------------------

  if(cloud != nullptr){
    //Set lidar model
    cloud->lidar_model = lidar_model;

    //Set scaling
    if(cloud_scale != 1){
      Transformation transformManager;
      transformManager.make_scaling(cloud, (float)cloud_scale);
      sceneManager->update_cloud_location(cloud);
    }
  }

  //---------------------------
}
void GUI_Initialization::operation_option(){
  //---------------------------

  //Remove old clouds
  ImGui::Checkbox("Remove clouds", &remove_cloud);

  //Lidar model
  static int lidar_model_id = 0;
  ImGui::SetNextItemWidth(100);
  if(ImGui::Combo("Lidar", &lidar_model_id, "velodyne_vlp16\0velodyne_hdl32\0")){
    if(lidar_model_id == 0){
      this->lidar_model = "velodyne_vlp16";
    }else if(lidar_model_id == 1){
      this->lidar_model = "velodyne_hdl32";
    }
  }

  //Point cloud scaling
  ImGui::SetNextItemWidth(100);
  ImGui::DragInt("Scale", &cloud_scale, 1, 1, 100, "%d x");

  //---------------------------
}
