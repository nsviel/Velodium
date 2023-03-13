#include "MOD_filter.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"

#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Filter.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_filter::MOD_filter(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->filterManager = node_ope->get_filterManager();
  this->sceneManager = node_scene->get_sceneManager();

  this->item_width = 150;

  //---------------------------
}
MOD_filter::~MOD_filter(){}

//Main function
void MOD_filter::design_filter(){
  bool* open = &modal_tab.show_filter;
  if(*open){
    ImGui::Begin("Filter manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->filter_cylinder();
    this->filter_byAngle();
    this->filter_sphere();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}

//Specific function
void MOD_filter::filter_cylinder(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  if (ImGui::Button("Cylinder cleaning", ImVec2(item_width,0))){
    if(collection != nullptr){
      filterManager->filter_cylinder_cloud(collection);
    }
  }
  float* r_min = filterManager->get_cyl_r_min();
  float* r_max = filterManager->get_cyl_r_max();
  float* z_min = filterManager->get_cyl_z_min();
  ImGui::SameLine(); ImGui::SetNextItemWidth(100);
  ImGui::InputFloat("r min", r_min, 0.1f, 1.0f, "%.2f");
  ImGui::Dummy(ImVec2(item_width, 0.0f)); ImGui::SameLine(); ImGui::SetNextItemWidth(100);
  ImGui::InputFloat("r max", r_max, 0.1f, 1.0f, "%.2f");
  ImGui::Dummy(ImVec2(item_width, 0.0f)); ImGui::SameLine(); ImGui::SetNextItemWidth(100);
  ImGui::InputFloat("z min", z_min, 0.1f, 1.0f, "%.2f");

  //---------------------------
}
void MOD_filter::filter_byAngle(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  static int maxAngle = 80;
  if(ImGui::Button("Filter by angle", ImVec2(item_width,0))){
    if(collection != nullptr){

      list<Collection*>* list_collection = sceneManager->get_list_col_object();
      for(int i=0; i<list_collection->size(); i++){
        Collection* collection = *next(list_collection->begin(),i);
        filterManager->filter_maxAngle(collection, maxAngle);
        //sceneManager->update_buffer_location(collection);
      }

    }
  }
  ImGui::SameLine();
  ImGui::SetNextItemWidth(item_width);
  ImGui::DragInt("##1", &maxAngle, 1, 0, 90, "%d°");

  //---------------------------
}
void MOD_filter::filter_sphere(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  //---------------------------

  if(ImGui::Button("Clean sphere cloud", ImVec2(item_width,0))){
    if(collection != nullptr){
      filterManager->filter_sphere();
    }
  }
  ImGui::SameLine();
  static float sphereDiameter = 0.139f;
  ImGui::SetNextItemWidth(item_width);
  if(ImGui::DragFloat("##6", &sphereDiameter, 0.0001, 0, 2, "%.5f")){
    filterManager->set_sphereDiameter(sphereDiameter);
  }

  //---------------------------
}
