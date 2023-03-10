#ifdef WITH_PCL
#ifndef SAMPLING_GUI_H
#define SAMPLING_GUI_H

#include "../../common.h"
#include "Sampling.h"

class GUI_sampling
{
public:
  //Constructor / Destructor
  GUI_sampling(){
    this->samplingManager = new Sampling();
  }
  ~GUI_sampling();

public:
  void sampling_gui(Collection* collection){
    int sizeButton = 150;

    //Random sampling
    if(ImGui::Button("Random sampling", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        Cloud* cloud = (Cloud*)collection->selected_obj;
        samplingManager->sampling_random(cloud);
      }
    }
    ImGui::SameLine();
    static int percent = 50;
    ImGui::PushItemWidth(100);
    if(ImGui::DragInt("##2", &percent, 1, 0, 100, "%d%%")){
      samplingManager->set_sampling(percent);
    }

    //Space sampling
    static float resolution = 0.1f;
    if(ImGui::Button("Space sampling", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        Cloud* cloud = (Cloud*)collection->selected_obj;
        samplingManager->sampling_spaceRadius(cloud, resolution);
      }
    }
    ImGui::SameLine();
    ImGui::DragFloat("##5", &resolution, 0.001, 0, 100, "%.4f");

    //Outliers filtering
    if(ImGui::Button("Outlier sampling", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        Cloud* cloud = (Cloud*)collection->selected_obj;
        samplingManager->sampling_outlier(cloud);
      }
    }
    ImGui::SameLine();
    static float radius = 0.1;
    ImGui::PushItemWidth(100);
    if(ImGui::DragFloat("##4", &radius, 0.0001, 0, 2, "%.5f")){
      samplingManager->set_sampling_radius(radius);
    }

    //Statistical filtering
    if(ImGui::Button("Statistical sampling", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        Cloud* cloud = (Cloud*)collection->selected_obj;
        samplingManager->sampling_statistical(cloud);
      }
    }
    ImGui::SameLine();
    static float sampling_std = 1.0f;
    ImGui::PushItemWidth(100);
    if(ImGui::DragFloat("##5", &sampling_std, 0.0001, 0, 2, "%.5f")){
      samplingManager->set_samplingstd(sampling_std);
    }
  }

private:
  Sampling* samplingManager;

};

#endif
#endif
