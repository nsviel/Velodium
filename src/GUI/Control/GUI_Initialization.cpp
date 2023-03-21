#include "GUI_Initialization.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"
#include "../../Load/Processing/Pather.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Module/Node_module.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Info.h"
#include "../../Interface/Node_interface.h"
#include "../../Interface/Capture/Capture.h"

#include <filesystem>


//Constructor / Destructor
GUI_Initialization::GUI_Initialization(Node_gui* node_gui){
  //---------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();
  Node_interface* node_interface = node_engine->get_node_interface();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();
  this->pathManager = node_load->get_patherManager();
  this->configManager = node_engine->get_configManager();
  this->captureManager = node_interface->get_captureManager();
  this->transformManager = new Transformation();

  path_init_file.push_back("../media/engine/fastScene/buddha.pts");
  path_init_file.push_back("../media/engine/fastScene/cube.obj");
  path_init_file.push_back("../media/engine/fastScene/sphere.obj");
  path_init_file.push_back("/home/aeter/Desktop/Point_cloud/obj/planet/planet.obj");

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

  this->with_remove_cloud = true;
  this->with_onthefly = false;
  this->object_scale = 1;
  this->lidar_model = "velodyne_vlp64";

  //Open accepted formats
  this->accepted_format.push_back("pts");
  this->accepted_format.push_back("obj");
  this->accepted_format.push_back("ply");
  this->accepted_format.push_back("xyz");
  this->accepted_format.push_back("cbor");
  this->accepted_format.push_back("pcap");
  this->accepted_format.push_back("ptx");
  this->accepted_format.push_back("csv");
  this->accepted_format.push_back("las");
  this->accepted_format.push_back("laz");

  //Tree file pathes
  this->path_init_vec.push_back(configManager->parse_json_s("interface", "path_point_cloud_1"));
  this->path_init_vec.push_back(configManager->parse_json_s("interface", "path_point_cloud_2"));
  this->path_init_vec.push_back(configManager->parse_json_s("interface", "path_point_cloud_3"));

  //---------------------------
  this->construst_tree();
}

//Operation on loaded cloud
void GUI_Initialization::operation_new_collection(Collection* collection){
  //---------------------------

  if(collection != nullptr){
    //Set lidar model
    collection->lidar_model = lidar_model;

    //Set scaling
    if(object_scale != 1){
      for(int i=0; i<collection->list_obj.size(); i++){
        Object_* object = collection->get_obj(i);
        sceneManager->update_MinMax(object);
        transformManager->make_scaling(object, object_scale);
        sceneManager->update_buffer_location(object);
      }
    }
  }

  //---------------------------
}
void GUI_Initialization::operation_option(){
  //---------------------------

  //Point cloud scaling
  ImGui::SetNextItemWidth(100);
  ImGui::DragFloat("Scale##4567", &object_scale, 0.1, 0.1, 100, "%.2f x");
  ImGui::SameLine();

  //Remove old clouds
  ImGui::Checkbox("Remove##222", &with_remove_cloud);

  //Lidar model
  static int lidar_model_id;
  if(lidar_model == "velodyne_vlp16"){
    lidar_model_id = 0;
  }else if(lidar_model == "velodyne_vlp64"){
    lidar_model_id = 1;
  }else if(lidar_model == "velodyne_hdl32"){
    lidar_model_id = 2;
  }else if(lidar_model == "velodyne_vlp16_reduced"){
    lidar_model_id = 3;
  }
  ImGui::SetNextItemWidth(100);
  if(ImGui::Combo("Lidar", &lidar_model_id, "vlp_16\0vlp_64\0hdl_32\0vlp_16_reduced\0")){
    if(lidar_model_id == 0){
      this->lidar_model = "velodyne_vlp16";
    }else if(lidar_model_id == 1){
      this->lidar_model = "velodyne_vlp64";
    }else if(lidar_model_id == 2){
      this->lidar_model = "velodyne_hdl32";
    }else if(lidar_model_id == 3){
      this->lidar_model = "velodyne_vlp16_reduced";
    }
  }
  ImGui::SameLine();

  //Remove old cloud
  ImGui::Checkbox("On the fly##222", &with_onthefly);

  //---------------------------
}

//Specific scene construction
void GUI_Initialization::construct_node_scene(vector<vector<tree_file*>>& nodes_path_vec){
  //---------------------------

  //Scene folder
  vector<tree_file*> root_scene;
  tree_file* node = new tree_file();
  node->name = "scene";
  node->type = "Folder";
  node->end_folder = true;
  node->leaf_nb = 3;
  node->leaf_idx = 1;
  node->already_open = true;
  root_scene.push_back(node);

  //Scene 1
  node = new tree_file();
  node->name = "Rocks";
  node->leaf_nb = 0;
  node->type = "scene_1";
  root_scene.push_back(node);

  //Scene 2
  node = new tree_file();
  node->name = "Bunny";
  node->leaf_nb = 0;
  node->type = "scene_2";
  root_scene.push_back(node);

  //Scene 3
  node = new tree_file();
  node->name = "Dragon";
  node->leaf_nb = 0;
  node->type = "scene_3";
  root_scene.push_back(node);

  nodes_path_vec.push_back(root_scene);

  //---------------------------
}
void GUI_Initialization::build_scene_1(){
  //---------------------------

  for (int i=0; i<3; i++){
    Collection* rock = loaderManager->load_collection("/home/aeter/Desktop/Point_cloud/obj/rock/rock.obj");
    transformManager->make_translation(rock, vec3(2-4*i, 0, 0));
    sceneManager->update_collection_location(rock);
  }

  for (int i=0; i<3; i++){
    Collection* rock = loaderManager->load_collection("/home/aeter/Desktop/Point_cloud/obj/rock/rock.obj");
    transformManager->make_translation(rock, vec3(2-4*i, -4, 0));
    sceneManager->update_collection_location(rock);
  }

  //---------------------------
}
void GUI_Initialization::build_scene_2(){
  //---------------------------

  Collection* rabbit = loaderManager->load_collection("../media/engine/fastScene/bunny.ply");


  //---------------------------
}
void GUI_Initialization::build_scene_3(){
  //---------------------------

  Collection* dragon = loaderManager->load_collection("/home/aeter/Desktop/Point_cloud/ply/dragon.ply");


  //---------------------------
}

//Tree view
void GUI_Initialization::treeview(){
  //---------------------------

  static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;
  if (ImGui::BeginTable("init_tree", 3, flags)){
    // The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide, 175);
    ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 75);
    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 75);
    ImGui::TableHeadersRow();

    //Display pre-built trees
    this->display_node_root(nodes_root);
    for(int i=0; i<nodes_path_vec.size(); i++){
      this->display_node(nodes_path_vec[i][0], nodes_path_vec[i]);
    }

    ImGui::EndTable();
  }

  //---------------------------
}
void GUI_Initialization::construst_tree(){
  this->nodes_path_vec.clear();
  //---------------------------

  //Construct init path nodes for specific cloud locations
  this->construct_node_root(path_init_file, nodes_root);

  //Set a node for specific scene construction
  this->construct_node_scene(nodes_path_vec);

  //Construct predefined init path nodes
  for(int i=0; i<path_init_vec.size(); i++){
    vector<tree_file*> nodes_path;
    this->construct_node(path_init_vec[i], nodes_path);
    this->nodes_path_vec.push_back(nodes_path);
  }

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
    node->size = get_file_size(path);
    nodes.push_back(node);

    //Recussively check all child leafs
    this->node_child_scan(path, nodes, node);

    //---------------------------
  }
}
void GUI_Initialization::construct_node_root(vector<string>& vec_path, vector<tree_file*>& nodes){
  //---------------------------

  for(int i=0; i<vec_path.size(); i++){
    if(vec_path[i] != ""){
      tree_file* node = new tree_file();

      node->name = get_filename_from_path(vec_path[i]);
      node->type = get_type_from_path(vec_path[i]);
      node->path = vec_path[i];
      node->size = get_file_size(vec_path[i]);
      node->leaf_nb = 1;
      node->leaf_idx = 1;
      node->already_open = true;
      node->end_folder = true;

      nodes.push_back(node);
    }
  }

  //---------------------------
}
void GUI_Initialization::node_child_scan(string path, vector<tree_file*>& nodes, tree_file* parent){
  vector<string> list_path = list_all_path(path);
  //---------------------------

  bool is_sub_folder = false;
  int id = nodes.size();
  parent->leaf_nb = list_path.size();
  parent->leaf_idx = id;
  parent->already_open = true;

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
      is_sub_folder = true;
      node->leaf_nb = 1;
      nodes.push_back(node);
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
        if(node->already_open == false){
          this->node_child_scan(node->path, all_nodes, node);
        }
        this->open_selection(node);
      }
      ImGui::TableNextColumn();
      ImGui::TextDisabled("--");
      ImGui::TableNextColumn();
      ImGui::TextUnformatted(node->type.c_str());
      if(open){
        if(node->already_open == false){
          this->node_child_scan(node->path, all_nodes, node);
        }
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
void GUI_Initialization::display_node_root(vector<tree_file*>& all_nodes){
  //---------------------------

  for(int i=0; i<all_nodes.size(); i++){
    tree_file* node = all_nodes[i];

    ImGui::TableNextRow();
    ImGui::TableNextColumn();

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

  if(with_remove_cloud){
    captureManager->stop_capture();
    sceneManager->remove_collection_all();
  }

  if(node->type == "File"){
    Collection* collection = loaderManager->load_collection(node->path);
    this->operation_new_collection(collection);
  }
  else if(node->type == "Folder" && node->end_folder){
    if(pathManager->check_folder_format(node->path, "ply")){
      Collection* collection;

      if(with_onthefly == false){
        collection = pathManager->loading_directory_frame(node->path);
      }else{
        collection = pathManager->loading_onthefly(node->path);
      }

      this->operation_new_collection(collection);
    }
  }
  else if(node->type == "scene_1"){
    this->build_scene_1();
  }
  else if(node->type == "scene_2"){
    this->build_scene_2();
  }
  else if(node->type == "scene_3"){
    this->build_scene_3();
  }

  //---------------------------
}
