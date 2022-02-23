#include "GUI_FileManager.h"

#include "../GUI_node.h"
#include "../Windows/Window_table.h"
#include "../Windows/GUI_windows.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/OpenGL/Dimension.h"

#include "../../Engine/Scene/Scene.h"

#include "IconsFontAwesome5.h"

extern struct Window_tab window_tab;


//Constructor / Destructor
GUI_fileManager::GUI_fileManager(GUI_node* node_gui){
  //-------------------------------

  Engine_node* node_engine = node_gui->get_node_engine();

  this->gui_winManager = node_gui->get_gui_winManager();
  this->dimManager = node_engine->get_dimManager();
  this->sceneManager = new Scene();

  //-------------------------------
}
GUI_fileManager::~GUI_fileManager(){}

//Subfunctions
void GUI_fileManager::fileManager(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //-------------------------------

  static ImVector<int> selection;
  static ImGuiTableFlags flags;
  flags |= ImGuiTableFlags_Resizable;
  flags |= ImGuiTableFlags_Borders;
  flags |= ImGuiTableFlags_ScrollY;
  flags |= ImGuiTableFlags_RowBg;
  flags |= ImGuiTableFlags_SizingFixedFit;
  flags |= ImGuiTableFlags_NoBordersInBody;

  static ImGuiSelectableFlags flags_selec;
  flags_selec |= ImGuiSelectableFlags_SpanAllColumns;
  flags_selec |= ImGuiSelectableFlags_AllowItemOverlap;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 255, 255, 255));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 255, 255, 255));
  if (ImGui::BeginTable("table_advanced", 3, flags, ImVec2(0, 0), 0)){
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 135);
    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, 20);
    ImGui::TableSetupColumn("Delete", ImGuiTableColumnFlags_WidthFixed, 20);

    for (int row_i=0; row_i<list_cloud->size(); row_i++){
      Cloud* cloud = *next(list_cloud->begin(), row_i);
      ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.5);
      ImGui::PushItemWidth(-FLT_MIN);flags |= ImGuiTreeNodeFlags_OpenOnArrow;
      ImGui::PushID(row_i);
      //----------

      ImGui::Selectable(cloud->name.c_str(), true, flags_selec);


      //First column
      ImGui::TableSetColumnIndex(0);
      this->cloudManager(cloud);

      //Second column
      ImGui::TableSetColumnIndex(1);
      if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
        sceneManager->selection_setCloud(cloud);
        window_tab.show_modifyFileInfo = !window_tab.show_modifyFileInfo;
      }

      //Third column
      ImGui::TableSetColumnIndex(2);
      if(ImGui::SmallButton(ICON_FA_TRASH)){
        sceneManager->remove_cloud(cloud);
      }

      //----------
      ImGui::PopItemWidth();
      ImGui::PopID();
    }

    ImGui::EndTable();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
  }

  //-------------------------------
}
void GUI_fileManager::cloudManager(Cloud* cloud){
  Cloud* cloud_selected = sceneManager->get_cloud_selected();
  //-------------------------------

  ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
  if(cloud_selected->oID == cloud->oID){
    node_flags = node_flags | ImGuiTreeNodeFlags_Selected;
  }
  bool open_cloud_node = ImGui::TreeNodeEx(cloud->name.c_str(), node_flags);

  //If clicked by mouse
  if(ImGui::IsItemClicked()){
    sceneManager->selection_setCloud(cloud);
  }

  //Subset tree node
  if(open_cloud_node && cloud != nullptr){

    for(int j=0; j<cloud->subset.size(); j++){
      Subset* subset = *next(cloud->subset.begin(), j);

      if(subset->visibility){
        node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_Selected;
      }else{
        node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      }

      bool open_subset_node = ImGui::TreeNodeEx(subset->name.c_str(), node_flags);

      if(open_subset_node){
        this->info_subset(subset);
        ImGui::TreePop();
      }

      //If clicked by mouse
      if(ImGui::IsItemClicked()){
        sceneManager->selection_setSubset(cloud, j);
      }
    }

    ImGui::TreePop();
  }

  //-------------------------------

}

void GUI_fileManager::info_cloud(Cloud* cloud){
  //---------------------------

  //Additional info
  ImGui::Text("Format: %s", cloud->format.c_str());
  ImGui::Text("Frames: %d", (int)cloud->subset.size());
  ImGui::Text("Points: %d", cloud->nb_point);

  //---------------------------
}
void GUI_fileManager::info_subset(Subset* subset){
  vec3& COM = subset->COM;
  vec3& PCroot = subset->root;
  ImGui::Separator();
  //---------------------------

  //Additional info
  ImGui::Text("Points: %d", (int)subset->xyz.size());
  ImGui::Text("COM (%.2f, %.2f, %.2f)", COM.x, COM.y, COM.z);
  ImGui::Text("Z [%.2f; %.2f]", subset->min.z, subset->max.z);

  //---------------------------
  ImGui::Separator();
}
void GUI_fileManager::info_iconAction(Cloud* cloud){
  //---------------------------

  //Removal cross
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
  ImGui::PushID(cloud->oID);
  //ImGui::SameLine(ImGui::GetWindowWidth()-40);
  if(ImGui::Button(ICON_FA_TRASH)){
    sceneManager->remove_cloud(cloud);
  }

  //Modification window
  ImGui::SameLine(ImGui::GetWindowWidth()-60);
  if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
    sceneManager->selection_setCloud(cloud);
    window_tab.show_modifyFileInfo = !window_tab.show_modifyFileInfo;
  }
  ImGui::PopID();
  ImGui::PopStyleColor();

  //---------------------------
}
