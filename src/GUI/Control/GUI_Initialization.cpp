#include "GUI_Initialization.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Module/Node_module.h"
#include "../../Specific/fct_system.h"

#include <filesystem>


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();
  this->pathManager = node_load->get_patherManager();
  this->configManager = node_engine->get_configManager();

  //---------------------------
  this->update_configuration();
}
GUI_Initialization::~GUI_Initialization(){}

//Main functions
void GUI_Initialization::init_gui(){
  //---------------------------

  this->operation_option();
  this->treeview();

  //---------------------------
}
void GUI_Initialization::update_configuration(){
  //---------------------------

  this->remove_cloud = true;
  this->cloud_scale = 1;
  this->lidar_model = "velodyne_vlp16";
  this->accepted_format.push_back("pts");
  this->accepted_format.push_back("obj");
  this->accepted_format.push_back("ply");
  this->accepted_format.push_back("xyz");
  this->accepted_format.push_back("cbor");
  this->accepted_format.push_back("pcap");
  this->accepted_format.push_back("ptx");
  this->accepted_format.push_back("csv");

  this->path_1 = configManager->parse_json_s("interface", "path_point_cloud_1");
  this->path_2 = configManager->parse_json_s("interface", "path_point_cloud_2");
  this->path_3 = configManager->parse_json_s("interface", "path_point_cloud_3");

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

  //Point cloud scaling
  ImGui::SetNextItemWidth(100);
  ImGui::DragInt("Scale", &cloud_scale, 1, 1, 100, "%d x");
  ImGui::SameLine();

  //Lidar model
  static int lidar_model_id = 0;
  ImGui::SetNextItemWidth(110);
  if(ImGui::Combo("Lidar", &lidar_model_id, "vlp16\0vlp64\0hdl32\0")){
    if(lidar_model_id == 0){
      this->lidar_model = "velodyne_vlp16";
    }else if(lidar_model_id == 1){
      this->lidar_model = "velodyne_vlp64";
    }else if(lidar_model_id == 2){
      this->lidar_model = "velodyne_hdl32";
    }
  }

  //Remove old clouds
  ImGui::Checkbox("Remove##222", &remove_cloud);

  //---------------------------
}

//Tree view
void GUI_Initialization::treeview(){
  //---------------------------

  static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
  if (ImGui::BeginTable("3ways", 3, flags)){
    // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide, 150);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 50);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 75);
    ImGui::TableHeadersRow();

    //Get directory paths
    vector<tree_file*> nodes_path_1;
    this->construct_tree(path_1, nodes_path_1);
    this->display_node(nodes_path_1[0], nodes_path_1);

    vector<tree_file*> nodes_path_2;
    this->construct_tree(path_2, nodes_path_2);
    this->display_node(nodes_path_2[0], nodes_path_2);

    if(path_3 != ""){
      vector<tree_file*> nodes_path_3;
      this->construct_tree(path_3, nodes_path_3);
      this->display_node(nodes_path_3[0], nodes_path_3);
    }

    ImGui::EndTable();
  }

  //---------------------------
}
void GUI_Initialization::display_node(tree_file* node, vector<tree_file*>& all_nodes){
  //---------------------------

  ImGui::TableNextRow();
  ImGui::TableNextColumn();
  const bool is_folder = (node->leaf_nb > 0);
  if(is_folder){
    bool open = ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
    if(ImGui::IsItemClicked()){
      this->open_selection(node);
    }
    ImGui::TableNextColumn();
    ImGui::TextDisabled("--");
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(node->type.c_str());
    if(open){
      for(int i=0; i<node->leaf_nb; i++){
        int id = node->leaf_idx + i;
        display_node(all_nodes[id], all_nodes);
      }
      ImGui::TreePop();
    }
  }
  else if(node->type != "Folder"){
    ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth);
    if(ImGui::IsItemClicked()){
      this->open_selection(node);
    }
    ImGui::TableNextColumn();
    ImGui::Text("%.2f", node->size);
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(node->type.c_str());
  }

  //---------------------------
}
void GUI_Initialization::construct_tree(string path, vector<tree_file*>& nodes){
  vector<string> list_path = list_allPaths(path);
  //---------------------------

  //Save root
  tree_file* truc = new tree_file();
  truc->name = get_filename_from_path(path);
  truc->type = get_type_from_path(path);
  nodes.push_back(truc);

  this->recursive_scan(path, nodes, truc);

  //---------------------------
}
void GUI_Initialization::recursive_scan(string path, vector<tree_file*>& nodes, tree_file* parent){
  vector<string> list_path = list_allPaths(path);
  //---------------------------

  int id = nodes.size();
  parent->leaf_nb = list_path.size();
  parent->leaf_idx = id;

  for(int i=0; i<list_path.size(); i++){
    string path_file = list_path[i];

    tree_file* truc = new tree_file();
    truc->name = get_filename_from_path(path_file);
    truc->type = get_type_from_path(path_file);
    truc->path = path_file;

    if(truc->type != "Folder"){
      if(check_file_format(path_file)){
        truc->size = get_file_size(path_file);
        nodes.push_back(truc);
      }else{
        parent->leaf_nb--;
        delete truc;
      }
    }else{
      nodes.push_back(truc);
    }
  }

  bool is_sub_folder = false;
  for(int i=id; i<nodes.size(); i++){
    tree_file* node = nodes[i];
    if(node->type == "Folder"){
      is_sub_folder = true;
      recursive_scan(node->path, nodes, node);
    }
  }
  if(is_sub_folder){
    parent->end_folder = false;
  }else{
    parent->end_folder = true;
  }

  //---------------------------
}
bool GUI_Initialization::check_file_format(string path){
  string format = get_format_from_path(path);
  //---------------------------

  for(int i=0; i<accepted_format.size(); i++){
    if(format == accepted_format[i]){
      return true;
    }
  }

  //---------------------------
  return false;
}
void GUI_Initialization::open_selection(tree_file* node){
  //---------------------------

  if(node->type == "File"){
    if(remove_cloud){
      sceneManager->remove_cloud_all();
    }
    if(loaderManager->load_cloud(node->path)){
      this->operation_cloud(loaderManager->get_createdcloud());
    }
  }
  else if(node->type == "Folder" && node->end_folder){
    if(pathManager->check_folder_format(node->path, "ply")){
      if(remove_cloud){
        sceneManager->remove_cloud_all();
      }
      if(pathManager->loading_directory_frame(node->path)){
        this->operation_cloud(loaderManager->get_createdcloud());
      }
    }
  }


  //---------------------------
}
