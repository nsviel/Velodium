#include "MOD_transformation.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Transformation.h"
#include "../../../Operation/Transformation/Pose.h"
#include "../../../Specific/fct_transtypage.h"
#include "../../../Specific/fct_math.h"

#include "../Modal_tab.h"
extern struct Modal_tab modal_tab;


//Constructor / Destructor
MOD_transformation::MOD_transformation(Node_engine* node_engine){
  //---------------------------

  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->poseManager = new Pose();
  this->transformManager = new Transformation();

  this->item_width = 150;
  this->ground = 0.0f;

  //---------------------------
}
MOD_transformation::~MOD_transformation(){}

//Main function
void MOD_transformation::design_transformation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  if(modal_tab.show_transformation && cloud != nullptr){
    ImGui::Begin("Transformation", &modal_tab.show_transformation, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->actual_transformation_mat();
    this->manual_transformation_mat();
    this->cloud_translation();
    this->cloud_rotation();
    this->cloud_elevation();

    //---------------------------
    if(ImGui::Button("Close")){
      modal_tab.show_transformation = false;
    }
    ImGui::End();
  }
}

//Specific function
void MOD_transformation::actual_transformation_mat(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Actual transformation from initial position");
  MatrixXf cloudTransformation = glm_to_eigen_mat4_matXf(subset->transformation);
  std::stringstream ss;
  ss << cloudTransformation;
  string bla = ss.str();
  static char cloudTransformation_c[1024 * 16];
  strcpy(cloudTransformation_c, bla.c_str());
  static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine;
  ImGui::InputTextMultiline("##source", cloudTransformation_c, IM_ARRAYSIZE(cloudTransformation_c), ImVec2(300, ImGui::GetTextLineHeight() * 6), flags);

  //---------------------------
}
void MOD_transformation::manual_transformation_mat(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;

  if(ImGui::CollapsingHeader("Manual transformation matrix")){
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Applicable transformation matrix");
    static char TransfoMatrix[1024 * 16] =
      "1.000 0.000 0.000 0.000\n"
      "0.000 1.000 0.000 0.000\n"
      "0.000 0.000 1.000 0.000\n"
      "0.000 0.000 0.000 1.000\n";

    if(ImGui::Button("Reset##2", ImVec2(100,0))){
      strcpy(TransfoMatrix,
          "1.000 0.000 0.000 0.000\n"
          "0.000 1.000 0.000 0.000\n"
          "0.000 0.000 1.000 0.000\n"
          "0.000 0.000 0.000 1.000\n");
    }
    ImGui::SameLine();
    if(ImGui::Button("Transpose", ImVec2(100,0))){
      //Convert char* to string
      string str(TransfoMatrix);

      //Convert string to MatrixXf
      istringstream iss(str);
      float m0, m1, m2, m3;
      float m4, m5, m6, m7;
      float m8, m9, m10, m11;
      float m12, m13, m14, m15;
      iss >> m0 >> m1 >> m2 >> m3;
      iss >> m4 >> m5 >> m6 >> m7;
      iss >> m8 >> m9 >> m10 >> m11;
      iss >> m12 >> m13 >> m14 >> m15;

      MatrixXf mat(4,4);
      mat(0,0) = m0; mat(0,1) = m1; mat(0,2) = m2; mat(0,3) = m3;
      mat(1,0) = m4; mat(1,1) = m5; mat(1,2) = m6; mat(1,3) = m7;
      mat(2,0) = m8; mat(2,1) = m9; mat(2,2) = m10; mat(2,3) = m11;
      mat(3,0) = m12; mat(3,1) = m13; mat(3,2) = m14; mat(3,3) = m15;
      MatrixXf mat2 = mat.transpose();

      //Copy result on the gui
      std::stringstream ss;
      ss << mat2;
      string bla = ss.str();
      strcpy(TransfoMatrix, bla.c_str());
    }

    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine;
    ImGui::InputTextMultiline("##source2", TransfoMatrix, IM_ARRAYSIZE(TransfoMatrix), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

    if(ImGui::Button("Apply from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        Cloud* cloud = sceneManager->get_selected_cloud();
        Subset* subset = cloud->subset_selected;

        sceneManager->reset_cloud(cloud);
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Apply from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        //------------------
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }
    if(ImGui::Button("Reverse from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        Cloud* cloud = sceneManager->get_selected_cloud();
        Subset* subset = cloud->subset_selected;

        sceneManager->reset_cloud(cloud);
        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reverse from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        transformManager->make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void MOD_transformation::cloud_translation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;

  if(ImGui::CollapsingHeader("Cloud translation")){
    //---------------------------

    ImGui::PushItemWidth(150);
    static float trans[3] = {0.0f, 0.0f, 0.0f};
    ImGui::DragFloat3("XYZ", trans, 0.01f, -10.0f, 10.0f);
    ImGui::SameLine();
    if(ImGui::Button("Apply##1")){
      if(cloud != nullptr){
        vec3 translation = vec3(trans[0], trans[1], trans[2]);
        transformManager->make_translation(cloud, translation);
        sceneManager->update_cloud_location(cloud);
        trans[0] = 0;
        trans[1] = 0;
        trans[2] = 0;
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void MOD_transformation::cloud_rotation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;

  if(ImGui::CollapsingHeader("Cloud rotation")){
    //---------------------------

    if(ImGui::Button("X ->")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(90, 0, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("X <-")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(-90, 0, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Y ->")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(0, 90, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Y <-")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(0, -90, 0));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Z ->")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(0, 0, 90));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Z <-")){
      if(cloud != nullptr){
        vec3 radian = fct_degreeToRadian_vec3(vec3(0, 0, -90));
        transformManager->make_rotation(subset, subset->COM, radian);
        sceneManager->update_subset_location(subset);
      }
    }

    //---------------------------
    ImGui::Separator();
  }
}
void MOD_transformation::cloud_elevation(){
  if(ImGui::CollapsingHeader("Cloud elevation")){
    //---------------------------

    this->elevation_ground();
    this->elevation_height();
    this->elevation_redressment();

    //---------------------------
    ImGui::Separator();
  }
}

//Elevation function
void MOD_transformation::elevation_ground(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Ground");

  static float ground = 0.0f;
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Ground estimation: %.3f m", ground);
  static int ground_nb_point = 10000;
  ImGui::PushItemWidth(75);
  if(ImGui::DragInt("Ground number of points", &ground_nb_point, 100, 0, 1000000)){
    if(cloud != nullptr){
      poseManager->set_ground_nbPoint(ground_nb_point);
      ground = poseManager->make_soilDetermination(cloud);
    }
  }
  static float Zpos = 0.0f;
  ImGui::PushItemWidth(75);
  if(ImGui::DragFloat("Manual setting", &Zpos, 0.01f)){
    if(cloud != nullptr){
      poseManager->make_elevation(cloud, Zpos);
      sceneManager->update_cloud_location(cloud);
      ground = poseManager->make_soilDetermination(cloud);
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_transformation::elevation_height(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Elevation");

  //Scanner height from ground
  static float Z_scan = 0.0f;
  if(ImGui::DragFloat("Elevation", &Z_scan, 0.05f)){
    if(cloud != nullptr){
      ground = poseManager->make_soilDetermination(cloud);
    }
  }
  ImGui::SameLine();

  //One or all cloud to operate
  static bool allClouds = false;
  ImGui::Checkbox("All clouds", &allClouds);
  ImGui::SameLine();

  if(ImGui::Button("Apply##0")){
    if(allClouds){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);
        poseManager->make_adjustPosToScanner(cloud, Z_scan);
        sceneManager->update_cloud_location(cloud);
      }
    }
    else{
      if(cloud != nullptr){
        poseManager->make_adjustPosToScanner(cloud, Z_scan);
        sceneManager->update_cloud_location(cloud);
        ground = poseManager->make_soilDetermination(cloud);
      }
    }
  }

  //---------------------------
  ImGui::Separator();
}
void MOD_transformation::elevation_redressment(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Redressment");

  if(ImGui::Button("Plane fitting on selected points")){
    if(cloud != nullptr){
      poseManager->make_alignSelectionToGround(cloud);
      sceneManager->update_cloud_location(cloud);
    }
  }

  //---------------------------
  ImGui::Separator();
}
