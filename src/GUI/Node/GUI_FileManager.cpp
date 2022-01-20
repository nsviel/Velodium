#include "GUI_FileManager.h"

#include "../Windows/Window_tab.h"
#include "../Windows/GUI_windows.h"

#include "../../Engine/Configuration/Dimension.h"
#include "../../Engine/Scene.h"

#include "../../../extern/IconsFontAwesome5.h"

extern struct Window_tab window_tab;


//Constructor / Destructor
GUI_fileManager::GUI_fileManager(Dimension* dim, GUI_windows* win){
  this->gui_winManager = win;
  this->dimManager = dim;
  //-------------------------------

  this->sceneManager = new Scene();

  //-------------------------------
}
GUI_fileManager::~GUI_fileManager(){}

//Subfunctions
void GUI_fileManager::fileManager(){
  /*Cloud* cloud_selected = sceneManager->get_cloud_selected();
  //-------------------------------

  //Model tab
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;

  //Get list of all loaded files
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

  static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 255, 255, 255));
  if (ImGui::BeginTable("file manager", 1, flags, ImVec2(0.0f, 0.0f))){

    //Header stuff
    //ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
    //ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_None);
    //ImGui::TableHeadersRow();

    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

    //Cloud tree node
    ImGui::TableNextRow();
    ImGui::TableNextColumn();

    sceneManager->update_cloud_oID(list_cloud);

    static int is_selected = 0;
    for(int i=0; i<list_cloud->size(); i++){
      Cloud* cloud = *next(list_cloud->begin(),i);

      ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
      if(cloud_selected->oID == i){
        node_flags = node_flags | ImGuiTreeNodeFlags_Selected;
      }
      bool open_cloud_node = ImGui::TreeNodeEx(cloud->name.c_str(), node_flags);
      this->info_iconAction(cloud);

      //If clicked by mouse
      if(ImGui::IsItemClicked()){
        sceneManager->selection_setCloud(cloud);
      }

      //Subset tree node
      if(open_cloud_node && cloud != nullptr){

        for(int j=0; j<cloud->subset.size(); j++){
          Subset* subset = &cloud->subset[j];

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
    }

    ImGui::TableNextRow();
    ImGui::EndTable();
  }

  //-------------------------------
  ImGui::TreePop();
  ImGui::PopStyleColor();
  ImGui::PopStyleColor();*/

  tableManager();
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
      Subset* subset = &cloud->subset[j];

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
  ImGui::TreePop();
}
void GUI_fileManager::tableManager(){
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
    ImGui::TableSetupColumn("Delete", ImGuiTableColumnFlags_WidthFixed, 20);
    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, 20);

    for (int row_i=0; row_i<list_cloud->size(); row_i++){
      Cloud* cloud = *next(list_cloud->begin(), row_i);
      ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.5);
      ImGui::PushItemWidth(-FLT_MIN);flags |= ImGuiTreeNodeFlags_OpenOnArrow;
      ImGui::PushID(row_i);
      //----------

      ImGui::Selectable(cloud->name.c_str(), true, flags_selec);


      //First column
      ImGui::TableSetColumnIndex(0);
      //ImGui::Text("%s", cloud->name.c_str());
      cloudManager(cloud);
      //Second column
      ImGui::TableSetColumnIndex(1);
      if(ImGui::SmallButton(ICON_FA_TRASH)){
        sceneManager->removeCloud(cloud);
      }

      //Third column
      ImGui::TableSetColumnIndex(2);
      if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
        sceneManager->selection_setCloud(cloud);
        window_tab.show_modifyFileInfo = !window_tab.show_modifyFileInfo;
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

  sceneManager->update_subset_dataFormat(subset);

  //Additional info
  ImGui::Text("Points: %d", (int)subset->xyz.size());
  ImGui::Text("Format: %s", subset->dataFormat.c_str());
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
    sceneManager->removeCloud(cloud);
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
