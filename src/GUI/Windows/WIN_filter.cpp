#include "WIN_filter.h"

#include "../../Engine/Scene.h"
#include "../../Operation/Transformation/Filter.h"

#include "Window_tab.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_filter::WIN_filter(Filter* filter){
  //---------------------------

  this->filterManager = filter;
  this->sceneManager = new Scene();

  this->item_width = 150;

  //---------------------------
}
WIN_filter::~WIN_filter(){}

//Main function
void WIN_filter::window_filter(){
  bool* open = &window_tab.show_filter;
  if(*open){
    ImGui::Begin("Filter manager", open, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Cylinder filtering
    if (ImGui::Button("Cylinder cleaning", ImVec2(item_width,0))){
      if(cloud != nullptr){
        filterManager->filter_cloud_cylinder(cloud);
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

    //Filter by angle
    static int maxAngle = 80;
    if(ImGui::Button("Filter by angle", ImVec2(item_width,0))){
      if(cloud != nullptr){

        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);
          filterManager->filter_maxAngle(cloud, maxAngle);
          sceneManager->update_cloud_location(cloud);
        }

      }
    }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(item_width);
    ImGui::DragInt("##1", &maxAngle, 1, 0, 90, "%d°");

    //Sphere filtering
    if(ImGui::Button("Clean sphere cloud", ImVec2(item_width,0))){
      if(cloud != nullptr){
        filterManager->filter_sphereCleaning();
      }
    }
    ImGui::SameLine();
    static float sphereDiameter = 0.139f;
    ImGui::SetNextItemWidth(item_width);
    if(ImGui::DragFloat("##6", &sphereDiameter, 0.0001, 0, 2, "%.5f")){
      filterManager->set_sphereDiameter(sphereDiameter);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}
