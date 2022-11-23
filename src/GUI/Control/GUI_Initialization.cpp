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
  this->construst_tree();
}

//GUI subfunctions
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
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide, 175);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 75);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 75);
    ImGui::TableHeadersRow();

    //Display pre-built trees
    this->display_node(nodes_path_1[0], nodes_path_1);
    this->display_node(nodes_path_2[0], nodes_path_2);
    this->display_node(nodes_path_3[0], nodes_path_3);

    ImGui::EndTable();
  }

  //---------------------------
}
void GUI_Initialization::construst_tree(){
  //---------------------------

  this->construct_node(path_1, nodes_path_1);
  this->construct_node(path_2, nodes_path_2);
  this->construct_node(path_3, nodes_path_3);

  //---------------------------
}
void GUI_Initialization::construct_node(string path, vector<tree_file*>& nodes){
  if(path != ""){
    //---------------------------

    //Save root
    tree_file* node = new tree_file();
    node->name = get_filename_from_path(path);
    node->type = get_type_from_path(path);
    node->path = path;
    nodes.push_back(node);

    this->node_child_scan(path, nodes, node);

    //---------------------------
  }
}
void GUI_Initialization::node_child_scan(string path, vector<tree_file*>& nodes, tree_file* parent){
  vector<string> list_path = list_allPaths(path);
  //---------------------------

  int id = nodes.size();
  parent->leaf_nb = list_path.size();
  parent->leaf_idx = id;

  for(int i=0; i<list_path.size(); i++){
    string path_file = list_path[i];

    tree_file* node = new tree_file();
    node->name = get_filename_from_path(path_file);
    node->type = get_type_from_path(path_file);
    node->path = path_file;

    if(node->type != "Folder"){
      if(check_file_format(path_file)){
        node->leaf_nb = 0;
        node->size = get_file_size(path_file);
        nodes.push_back(node);
      }else{
        parent->leaf_nb--;
        delete node;
      }
    }else{
      node->leaf_nb = 1;
      nodes.push_back(node);
    }
  }

  bool is_sub_folder = false;
  for(int i=id; i<nodes.size(); i++){

    tree_file* node = nodes[i];
    if(node->type == "Folder"){
      is_sub_folder = true;
    }
  }
  if(is_sub_folder){
    parent->end_folder = false;
  }else{
    parent->end_folder = true;
  }

  //---------------------------
}
void GUI_Initialization::recursive_folder(string path, vector<tree_file*>& nodes, tree_file* parent){
  //---------------------------

  int id = nodes.size();
  bool is_sub_folder = false;
  for(int i=id; i<nodes.size(); i++){

    tree_file* node = nodes[i];
    if(node->type == "Folder"){
      is_sub_folder = true;
      node_child_scan(node->path, nodes, node);
    }
  }
  if(is_sub_folder){
    parent->end_folder = false;
  }else{
    parent->end_folder = true;
  }

  //---------------------------
}
void GUI_Initialization::display_node(tree_file* node, vector<tree_file*>& all_nodes){
  if(all_nodes.size() != 0){
    //---------------------------

    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    const bool is_folder = (node->leaf_nb > 0);
    if(is_folder){

      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
      bool open = ImGui::TreeNodeEx(node->name.c_str(), node_flags);
      if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
        this->open_selection(node);
      }
      ImGui::TableNextColumn();
      ImGui::TextDisabled("--");
      ImGui::TableNextColumn();
      ImGui::TextUnformatted(node->type.c_str());
      if(open){
        this->node_child_scan(node->path, all_nodes, node);
        for(int i=0; i<node->leaf_nb; i++){
          int id = node->leaf_idx + i;
          display_node(all_nodes[id], all_nodes);
        }
        ImGui::TreePop();
      }
    }
    else if(node->type != "Folder"){
      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanFullWidth;
      ImGui::TreeNodeEx(node->name.c_str(), node_flags);
      if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)){
        this->open_selection(node);
      }
      ImGui::TableNextColumn();
      ImGui::Text("%.1f MB", node->size);
      ImGui::TableNextColumn();
      ImGui::TextUnformatted(node->type.c_str());
    }

    //---------------------------
  }
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
