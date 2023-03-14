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
  this->data = Data::get_instance();

  //-------------------------------
}
GUI_fileManager::~GUI_fileManager(){}

//Subfunctions
void GUI_fileManager::data_tree(){
  list<Collection*>* list_collection = data->get_list_col_object();
  //-------------------------------

  static ImVector<int> selection;
  static ImGuiTableFlags flags;
  flags |= ImGuiTableFlags_ScrollY;
  flags |= ImGuiTableFlags_SizingFixedFit;
  flags |= ImGuiTableFlags_SizingStretchSame;

  static ImGuiSelectableFlags flags_selec;
  flags_selec |= ImGuiSelectableFlags_SpanAllColumns;
  flags_selec |= ImGuiSelectableFlags_AllowItemOverlap;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(250, 250, 250, 255));
  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));

  if (ImGui::BeginTable("table_advanced", 1, flags)){
    ImGui::TableSetupColumn("Name");

    //Light collection
    Collection* collection = data->get_collection_byName("glyph", "light");
    ImGui::TableNextRow();
    ImGui::PushID(0);
    ImGui::TableNextColumn();
    this->collection_node(collection);
    ImGui::PopID();

    //Object collection
    for (int row_i=0; row_i<list_collection->size(); row_i++){
      Collection* collection = *next(list_collection->begin(), row_i);
      if(collection->nb_obj == 0) continue;
      //----------

      //Set table row
      ImGui::TableNextRow();
      ImGui::PushID(row_i);

      //Cloud name
      ImGui::TableNextColumn();
      this->collection_node(collection);

      //----------
      ImGui::PopID();
    }

    ImGui::EndTable();
    ImGui::PopStyleColor();
    ImGui::PopStyleColor();
  }

  //-------------------------------
}
void GUI_fileManager::collection_node(Collection* collection){
  Collection* selected_col = data->get_selected_collection();
  if(selected_col == nullptr) {return;}
  //-------------------------------

  ImGuiTreeNodeFlags node_flags;
  node_flags |= ImGuiTreeNodeFlags_OpenOnArrow;
  node_flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
  if(selected_col->ID_col_order == collection->ID_col_order){
    node_flags |= ImGuiTreeNodeFlags_Selected;
  }

  bool is_node_open = ImGui::TreeNodeEx(collection->name.c_str(), node_flags);

  //If clicked by mouse
  if(ImGui::IsItemClicked()){
    data->set_selected_collection(collection);
  }

  //Subset tree node
  if(is_node_open && collection != nullptr && (collection->list_obj.size() > 0 || collection->is_onthefly)){

    this->info_collection(collection);

    for(int j=0; j<collection->list_obj.size(); j++){
      Object_* object = *next(collection->list_obj.begin(), j);

      bool is_node_open = ImGui::TreeNodeEx(object->name.c_str(), node_flags);

      if(is_node_open){
        this->info_object(object);
        ImGui::TreePop();
      }

      //If clicked by mouse
      if(ImGui::IsItemClicked()){
        graphManager->object_clicked(collection, j);
      }
    }

    ImGui::TreePop();
  }

  //-------------------------------

}

void GUI_fileManager::info_collection(Collection* collection){
  //---------------------------

  //Icon: info
  if(ImGui::Button(ICON_FA_CLIPBOARD)){
    data->set_selected_collection(collection);
    modal_tab.show_modifyFileInfo = !modal_tab.show_modifyFileInfo;
  }
  ImGui::SameLine();

  //Icon: delete
  if(ImGui::Button(ICON_FA_TRASH)){
    sceneManager->remove_collection(collection);
  }
  ImGui::SameLine();

  //Icon: visibility
  bool is_visible = collection->is_visible;
  if(ImGui::Checkbox("##444", &is_visible)){
    collection->set_visibility(is_visible);
  }
  ImGui::SameLine();

  //Additional info
  ImGui::Text("   %d", (int)collection->list_obj.size());

  //---------------------------
}
void GUI_fileManager::info_object(Object_* object){
  vec3& COM = object->COM;
  //ImGui::Separator();
  //---------------------------

  //Additional info
  ImGui::Text("Points: %d", (int)object->xyz.size());
  ImGui::Text("COM (%.2f, %.2f, %.2f)", COM.x, COM.y, COM.z);
  ImGui::Text("Z [%.2f; %.2f]", object->min.z, object->max.z);

  //---------------------------
  //ImGui::Separator();
}
void GUI_fileManager::info_iconAction(Collection* collection){
  //---------------------------

  //Removal cross
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(255, 0, 0, 255));
  ImGui::PushID(collection->ID_col_order);
  //ImGui::SameLine(ImGui::GetWindowWidth()-40);
  if(ImGui::Button(ICON_FA_TRASH)){
    sceneManager->remove_collection(collection);
  }

  //Modification window
  ImGui::SameLine(ImGui::GetWindowWidth()-60);
  if(ImGui::SmallButton(ICON_FA_CLIPBOARD)){
    data->set_selected_collection(collection);
    modal_tab.show_modifyFileInfo = !modal_tab.show_modifyFileInfo;
  }
  ImGui::PopID();
  ImGui::PopStyleColor();

  //---------------------------
}
