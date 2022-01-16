#include "WIN_loading.h"

#include "../../Load/Operation.h"
#include "../../Load/Loader.h"

#include "../../Load/Format/file_PTS.h"
#include "../../Load/Format/file_PTX.h"
#include "../../Load/Format/file_PCAP.h"

#include "Window_tab.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_loading::WIN_loading(){
  //---------------------------

  this->opeManager = new Operation();
  this->loaderManager = opeManager->get_loaderManager();

  this->item_width = 150;
  this->file_selected = false;
  this->file_path = "path/to/file";

  //---------------------------
}
WIN_loading::~WIN_loading(){}

//Main function
void WIN_loading::window_loading(){
  bool* open = &window_tab.show_loading;
  if(*open){
    ImGui::Begin("Loader manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->loading_action();
    this->loading_fileSelection();
    this->loading_dataFormat();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Load")){
      opeManager->loading();
      *open = false;
    }
    ImGui::SameLine();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}
void WIN_loading::window_saving(){
  bool* open = &window_tab.show_saving;
  if(*open){
    ImGui::Begin("Save", open, ImGuiWindowFlags_AlwaysAutoResize);
    static bool all = false;
    //---------------------------

    this->saving_action();
    this->saving_configuration();

    //---------------------------
    ImGui::End();
  }
}

//Sub functions
void WIN_loading::loading_action(){
  //---------------------------

  //Usual cloud loader
  if (ImGui::Button("Open cloud", ImVec2(item_width, 0))){
    opeManager->loading();
  }

  //Load a set of PLY binary frames
  if (ImGui::Button("Open PLY frames", ImVec2(item_width, 0))){
    opeManager->loading_frames();
  }

  //---------------------------
  ImGui::Separator();
}
void WIN_loading::loading_fileSelection(){
  //---------------------------

  //Get info for a specified file
  if (ImGui::Button("Select file", ImVec2(item_width, 0))){
    file_path = opeManager->get_filePath();
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
void WIN_loading::loading_retrieve_info(string file_path){
  file_PCAP* pcapManager = loaderManager->get_pcapManager();
  //---------------------------

  this->file_format = file_path.substr(file_path.find_last_of(".") + 1);

  //---------------------------
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
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity scaling");
      static int Idata = 2;
      static bool osef = false;
      if(ImGui::RadioButton("[0;1]", &Idata, 0)){
        ptxManager->set_IdataFormat(0);
        ptsManager->set_IdataFormat(0);
      }
      ImGui::SameLine();
      if(ImGui::RadioButton("[0;255]", &Idata, 1)){
        ptxManager->set_IdataFormat(1);
        ptsManager->set_IdataFormat(1);
      }
      ImGui::SameLine();
      if(ImGui::RadioButton("[-2048;2048]", &Idata, 2)){
        ptxManager->set_IdataFormat(2);
        ptsManager->set_IdataFormat(2);
      }
      ImGui::Separator();
    }

    //---------------------------
    ImGui::TreePop();
  }
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

void WIN_loading::saving_action(){
  //---------------------------

  if (ImGui::Button("Save as", ImVec2(item_width, 0))){
    opeManager->saving();
  }

  if (ImGui::Button("Save all", ImVec2(item_width, 0))){
    opeManager->saving_allCloud();
  }

  static int frame_b = 0;
  static int frame_e = 100;
  if (ImGui::Button("Save frames range", ImVec2(item_width, 0))){
    opeManager->saving_frameRange(frame_b, frame_e);
  }

  //---------------------------
}
void WIN_loading::saving_configuration(){
  //---------------------------

  /*ImGui::Text("Intensity scale");
  static int e = 3;
  if(ImGui::RadioButton("[0;1]", &e, 1)){
    loadconfigManager->save_option(e);
  }
  if(ImGui::RadioButton("[0;255]", &e, 2)){
    loadconfigManager->save_option(e);
  }
  if(ImGui::RadioButton("[-2048;2048]", &e, 3)){
    loadconfigManager->save_option(e);
  }*/

  //---------------------------
}
