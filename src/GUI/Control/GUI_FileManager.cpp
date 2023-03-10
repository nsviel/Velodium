#include "GUI_FileManager.h"

#include "../Node_gui.h"
#include "../Modal/Modal_tab.h"
#include "../Modal/GUI_Modal.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Engine/Core/Dimension.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Data/Graph.h"

#include "image/IconsFontAwesome5.h"

extern struct Modal_tab modal_tab;


//Constructor / Destructor
GUI_fileManager::GUI_fileManager(Node_gui* node_gui){
  //-------------------------------

  Node_engine* node_engine = node_gui->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->gui_window = node_gui->get_gui_window();
  this->dimManager = node_engine->get_dimManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->graphManager = node_scene->get_graphManager();

  //-------------------------------
}
GUI_fileManager::~GUI_fileManager(){}

//Subfunctions
void GUI_fileManager::fileManager(){
  list<Collection*>* list_collection = sceneManager->get_list_collection();
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
  if (ImGui::BeginTable("table_advanced", 4, flags, ImVec2(0, 0), 0)){
    ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 115);
    ImGui::TableSetupColumn("Visualization", ImGuiTableColumnFlags_WidthFixed, 20);
    ImGui::TableSetupColumn("Info", ImGuiTableColumnFlags_WidthFixed, 20);
    ImGui::TableSetupColumn("Delete", ImGuiTableColumnFlags_WidthFixed, 20);

    for (int row_i=0; row_i<list_collection->size(); row_i++){
      Collection* cloud = *next(list_collection->begin(), row_i);
      ImGui::TableNextRow(ImGuiTableRowFlags_None, 0.5);
      ImGui::PushItemWidth(-FLT_MIN);flags |= ImGuiTreeNodeFlags_OpenOnArrow;
      ImGui::PushID(row_i);
      //----------

      ImGui::Selectable(cloud->name.c_str(), true, flags_selec);

      //Cloud name
      ImGui::TableSetColumnIndex(0);
      this->cloudManager(cloud);

      //Icon: info
      ImGui::TableSetColumnIndex(1);
      if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
        graphManager->selection_setCloud(cloud);
        modal_tab.show_modifyFileInfo = !modal_tab.show_modifyFileInfo;
      }

      //Icon: delete
      ImGui::TableSetColumnIndex(2);
      if(ImGui::SmallButton(ICON_FA_TRASH)){
        sceneManager->remove_cloud(cloud);
      }

      //Icon: visualization
      ImGui::TableSetColumnIndex(3);
      ImGui::Checkbox("", &cloud->is_visible);

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
void GUI_fileManager::cloudManager(Collection* cloud){
  Collection* cloud_selected = sceneManager->get_selected_collection();
  if(cloud_selected == nullptr) {}
  //-------------------------------

  ImGuiTreeNodeFlags node_flags;
  node_flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
  if(cloud_selected->ID_col_order == cloud->ID_col_order){
    node_flags |= ImGuiTreeNodeFlags_Selected;
  }
  bool open_cloud_node = ImGui::TreeNodeEx(cloud->name.c_str(), node_flags);

  //If clicked by mouse
  if(ImGui::IsItemClicked()){
    graphManager->selection_setCloud(cloud);
  }

  //Subset tree node
  if(open_cloud_node && cloud != nullptr && (cloud->nb_object > 1 || cloud->is_onthefly)){

    for(int j=0; j<cloud->subset.size(); j++){
      Cloud* subset = (Cloud*)*next(cloud->subset.begin(), j);

      if(subset->is_visible){
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
        graphManager->selection_setSubset(cloud, j);
      }
    }

    ImGui::TreePop();
  }/*else if(open_cloud_node && cloud != nullptr && cloud->nb_object == 1){
    ImGui::TreePop();
  }*/

  //-------------------------------

}

void GUI_fileManager::info_cloud(Collection* cloud){
  //---------------------------

  //Additional info
  ImGui::Text("Format: %s", cloud->file_format.c_str());
  ImGui::Text("Frames: %d", (int)cloud->subset.size());
  ImGui::Text("Points: %d", cloud->nb_point);

  //---------------------------
}
void GUI_fileManager::info_subset(Cloud* subset){
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
void GUI_fileManager::info_iconAction(Collection* cloud){
  //---------------------------

  //Removal cross
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
  ImGui::PushID(cloud->ID_col_order);
  //ImGui::SameLine(ImGui::GetWindowWidth()-40);
  if(ImGui::Button(ICON_FA_TRASH)){
    sceneManager->remove_cloud(cloud);
  }

  //Modification window
  ImGui::SameLine(ImGui::GetWindowWidth()-60);
  if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
    graphManager->selection_setCloud(cloud);
    modal_tab.show_modifyFileInfo = !modal_tab.show_modifyFileInfo;
  }
  ImGui::PopID();
  ImGui::PopStyleColor();

  //---------------------------
}
