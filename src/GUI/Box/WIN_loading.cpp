#include "WIN_loading.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Pather.h"
#include "../../Load/Processing/Loader.h"

#include "../../Load/Format/file_PTS.h"
#include "../../Load/Format/file_PTX.h"
#include "../../Load/Format/file_PCAP.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_loading::WIN_loading(Node_engine* node_engine){
  //---------------------------

  Node_load* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();
  this->pathManager = node_load->get_pathManager();

  this->item_width = 150;
  this->file_selected = false;
  this->file_path = "path/to/file";
  this->save_mode = 0;
  this->load_mode = 0;
  this->subset_mode = 0;
  this->cloud_mode = 0;
  this->frame_b = 0;
  this->frame_e = 100;

  //---------------------------
}
WIN_loading::~WIN_loading(){}

//Main function
void WIN_loading::window_loading(){
  bool* open = &window_tab.show_loading;
  if(*open){
    ImGui::Begin("Loader manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->loading_specific();
    this->loading_custom_file();
    this->loading_dataFormat();

    //---------------------------
    this->loading_end(open);
  }
}
void WIN_loading::window_saving(){
  bool* open = &window_tab.show_saving;
  if(*open){
    ImGui::Begin("Save", open, ImGuiWindowFlags_AlwaysAutoResize);
    static bool all = false;
    //---------------------------

    this->saving_configuration();
    this->saving_dataFormat();

    //---------------------------
    this->saving_end(open);
  }
}

//Sub load functions
void WIN_loading::loading_specific(){
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Load options");
  ImGui::RadioButton("Cloud", &load_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Subset", &load_mode, 1);

  //---------------------------
  ImGui::Separator();
}
void WIN_loading::loading_custom_file(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Custom file");
  //---------------------------

  //Get info for a specified file
  if (ImGui::Button("Select file", ImVec2(item_width, 0))){
    file_path = pathManager->get_filePath();
    this->loading_retrieve_info(file_path);
    file_selected = true;
  }

  if(file_selected){
    //Display file path
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "%s", file_path.c_str());

    //File format
    static int format = 3;
    if(file_format == "pts"){
      format = 0;
    }
    else if(file_format == "ptx"){
      format = 1;
    }
    else if(file_format == "ply"){
      format = 2;
    }
    else if(file_format == "pcap"){
      format = 3;
    }
    ImGui::RadioButton("pts", &format, 0);
    ImGui::SameLine();
    ImGui::RadioButton("ptx", &format, 1);
    ImGui::SameLine();
    ImGui::RadioButton("ply", &format, 2);
    ImGui::SameLine();
    ImGui::RadioButton("pcap", &format, 3);

    //File format options
    if(format == 1){
      this->loading_file_ptx();
    }
    if(format == 3){
      this->loading_file_pcap();
    }

    //Open selected file
    if (ImGui::Button("Load selected", ImVec2(item_width, 0))){
      file_PCAP* pcapManager = loaderManager->get_pcapManager();
      loaderManager->load_cloud(file_path);
    }
  }

  //---------------------------
  ImGui::Separator();
}
void WIN_loading::loading_dataFormat(){
  if(ImGui::TreeNode("Data format")){
    file_PTX* ptxManager = loaderManager->get_ptxManager();
    file_PTS* ptsManager = loaderManager->get_ptsManager();
    //---------------------------

    //Data to  retrieve
    static bool data_intensity = true;
    if(ImGui::Checkbox("intensity", &data_intensity)){
      ptxManager->set_retrievingIntensity(data_intensity);
      ptsManager->set_retrievingIntensity(data_intensity);
    }
    ImGui::SameLine();
    static bool data_color = true;
    if(ImGui::Checkbox("color", &data_color)){
      ptxManager->set_retrievingColor(data_color);
      ptsManager->set_retrievingColor(data_color);
    }
    ImGui::SameLine();
    static bool data_normal = false;
    if(ImGui::Checkbox("normal", &data_normal)){
      ptxManager->set_retrievingNormal(data_normal);
      ptsManager->set_retrievingNormal(data_normal);
    }
    ImGui::SameLine();
    static bool data_ts = false;
    if(ImGui::Checkbox("timestamp", &data_ts)){

    }

    //Intensity data format
    if(data_intensity){
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity scale");
      static int I_mode = 2;
      if(ImGui::RadioButton("[0;1]", &I_mode, 0)){
        ptxManager->set_IdataFormat(0);
        ptsManager->set_IdataFormat(0);
      }
      ImGui::SameLine();
      if(ImGui::RadioButton("[0;255]", &I_mode, 1)){
        ptxManager->set_IdataFormat(1);
        ptsManager->set_IdataFormat(1);
      }
      ImGui::SameLine();
      if(ImGui::RadioButton("[-2048;2048]", &I_mode, 2)){
        ptxManager->set_IdataFormat(2);
        ptsManager->set_IdataFormat(2);
      }
    }

    //Intensity data format
    if(data_color){
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Color scale");
      static int rgb_mode = 1;
      if(ImGui::RadioButton("[0;1]", &rgb_mode, 0)){

      }
      ImGui::SameLine();
      if(ImGui::RadioButton("[0;255]", &rgb_mode, 1)){

      }
    }

    //---------------------------
    ImGui::TreePop();
  }
}
void WIN_loading::loading_end(bool* open){
  ImGui::Separator();
  //---------------------------

  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Load")){
    this->loading_action();
    *open = false;
  }
  ImGui::SameLine();
  if(ImGui::Button("Close")){
    *open = false;
  }
  ImGui::PopStyleColor(1);

  //---------------------------
  ImGui::End();
}

//sub sub laod functions
void WIN_loading::loading_retrieve_info(string file_path){
  file_PCAP* pcapManager = loaderManager->get_pcapManager();
  //---------------------------

  this->file_format = file_path.substr(file_path.find_last_of(".") + 1);

  //---------------------------
}
void WIN_loading::loading_file_ptx(){
  file_PTX* ptxManager = loaderManager->get_ptxManager();
  //---------------------------

  static int ptxMode = 1;
  if(ImGui::RadioButton("Scanner at origin", &ptxMode, 0)){
    ptxManager->set_scannerAtOrigin(true);
    ptxManager->set_applyCloudTransfo(false);
    ptxManager->set_separateCloud(false);
  }
  static bool cloudTransfo = false;
  if(ImGui::RadioButton("Apply cloud transformation", &ptxMode, 1)){
    ptxManager->set_scannerAtOrigin(false);
    ptxManager->set_applyCloudTransfo(true);
    ptxManager->set_separateCloud(false);
  }
  static bool separateClouds = true;
  if(ImGui::RadioButton("Separate clouds", &ptxMode, 2)){
    ptxManager->set_scannerAtOrigin(false);
    ptxManager->set_applyCloudTransfo(false);
    ptxManager->set_separateCloud(true);
  }
  static bool notUseZValue = false;
  if(ImGui::Checkbox("Not use Z value", &notUseZValue)){
    ptxManager->set_notUseZValue(notUseZValue);
  }

  //---------------------------
}
void WIN_loading::loading_file_pcap(){
  file_PCAP* pcapManager = loaderManager->get_pcapManager();
  //---------------------------

  //LiDAR model
  static int model = 0;
  if(ImGui::RadioButton("VLP-16", &model, 0)){
    pcapManager->set_lidar_model("vlp16");
  }
  ImGui::SameLine();
  if(ImGui::RadioButton("HDL-32", &model, 1)){
    pcapManager->set_lidar_model("hdl32");
  }

  //Get number of packets
  static int nb_packet = 1000000;
  if (ImGui::Button("Get number packet", ImVec2(item_width, 0))){
    nb_packet = pcapManager->get_file_length(file_path);
    cout<<"There are "<<nb_packet<<" packets in file"<<endl;
  }

  //Range of loaded packets
  int* ID_b = pcapManager->get_packet_beg();
  int* ID_e = pcapManager->get_packet_end();
  ImGui::SetNextItemWidth(item_width);
  string ID_max = "/" + to_string(nb_packet);
  if(ImGui::DragIntRange2(ID_max.c_str(), ID_b, ID_e, 1000, 0, nb_packet, "%d", "%d", nb_packet)){
    *pcapManager->get_packet_range_on() = true;
  }

  //---------------------------
}
void WIN_loading::loading_action(){
  //---------------------------

  //Load cloud
  if(load_mode == 0){
    pathManager->loading();
  }

  //Load subset
  if(load_mode == 1){
    pathManager->loading_frames();
  }

  //---------------------------
}

//Sub save functions
void WIN_loading::saving_configuration(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Save options");
  //---------------------------

  //Saving mode (Cloud / Subset)
  ImGui::RadioButton("Cloud", &save_mode, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Subset", &save_mode, 1);

  //Cloud mode (Selected / All)
  if(save_mode == 0){
    ImGui::RadioButton("Selected", &cloud_mode, 0);
    ImGui::SameLine();
    ImGui::RadioButton("All", &cloud_mode, 1);
  }

  //Subset mode (Selected / Range)
  if(save_mode == 1){
    ImGui::RadioButton("Selected", &subset_mode, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Range", &subset_mode, 1);
  }

  //Setup subset range
  if(save_mode == 1 && subset_mode == 1){
    //Retrieve max number of cloud subset
    int subset_max;
    if(sceneManager->get_is_list_empty()){
      subset_max = 0;
    }else{
      Cloud* cloud = sceneManager->get_selected_cloud();
      subset_max = cloud->nb_subset - 1;
    }

    //Drag range
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10); ImGui::SetNextItemWidth(item_width - 10);
    ImGui::DragIntRange2("##008", &frame_b, &frame_e, 0, 0, subset_max, "%d", "%d");
  }

  //---------------------------
  ImGui::Separator();
}
void WIN_loading::saving_dataFormat(){
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Save configuration");
  //---------------------------

  //File format
  static int file_format = 0;
  ImGui::RadioButton("pts", &file_format, 0);
  ImGui::SameLine();
  ImGui::RadioButton("ptx", &file_format, 1);
  ImGui::SameLine();
  ImGui::RadioButton("ply", &file_format, 2);

  //Data to  retrieve
  static bool with_intensity = true;
  static bool with_color = true;
  static bool with_normal = true;
  static bool with_timestamp = false;
  ImGui::Checkbox("Is", &with_intensity);
  ImGui::SameLine();

  //Intensity scale
  if(with_intensity){
    static int I_opt = 0;
    if(ImGui::RadioButton("[0;1]", &I_opt, 0)){

    }ImGui::SameLine();
    if(ImGui::RadioButton("[0;255]", &I_opt, 1)){

    }ImGui::SameLine();
    if(ImGui::RadioButton("[-2048;2048]", &I_opt, 2)){

    }
  }

  ImGui::Checkbox("RGB", &with_color);
  ImGui::SameLine();
  ImGui::Checkbox("Nxyz", &with_normal);
  ImGui::SameLine();
  ImGui::Checkbox("ts", &with_timestamp);

  //---------------------------
}
void WIN_loading::saving_action(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  //Save selected cloud
  if(save_mode == 0 && cloud_mode == 0 && !sceneManager->get_is_list_empty()){
    pathManager->saving_cloud(cloud);
  }

  //Save all cloud
  if(save_mode == 0 && cloud_mode == 1 && !sceneManager->get_is_list_empty()){
    pathManager->saving_cloud_all();
  }

  //Save selected subset
  if(save_mode == 1 && subset_mode == 0 && !sceneManager->get_is_list_empty()){
    Subset* subset = cloud->subset_selected;
    pathManager->saving_subset(subset);
  }

  //Save subset range
  if(save_mode == 1 && subset_mode == 1 && !sceneManager->get_is_list_empty()){
    pathManager->saving_subset_range(frame_b, frame_e);
  }

  //---------------------------
}
void WIN_loading::saving_end(bool* open){
  ImGui::Separator();
  //---------------------------

  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Save")){
    this->saving_action();
  }
  ImGui::SameLine();
  if(ImGui::Button("Close")){
    *open = false;
  }
  ImGui::PopStyleColor(1);

  //---------------------------
  ImGui::End();
}
