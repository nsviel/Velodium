#include "GUI_Registration.h"

#include "../Module_registration.h"
#include "../src/Pipeline/ICP.h"
#include "../src/Registration.h"


#include "../../Node_module.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Scene/Glyph/Glyphs.h"
#include "../../../Operation/Transformation/Pose.h"


//Constructor / Destructor
GUI_registration::GUI_registration(Module_registration* module){
  //---------------------------

  Node_engine* node_engine = module->get_node_engine();
  Node_module* node_module = module->get_node_module();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->glyphManager = node_scene->get_glyphManager();
  this->poseManager = new Pose();
  //this->regisManager = node_module->get_regisManager();
  //this->icpManager = regisManager->get_icpManager();

  this->regis_algo = 0;

  //---------------------------
}
GUI_registration::~GUI_registration(){}

//Main function
void GUI_registration::design_Registration(){
  //---------------------------

  this->regist_Registration();
  this->regist_Color();
  this->regist_Parameters();
  this->regist_DOF();
  this->regist_Stats();

  //---------------------------
}

//Subfunctions
void GUI_registration::regist_Color(){
  /*if(ImGui::CollapsingHeader("Colorization") && !sceneManager->get_is_list_empty()){
    int* colorMeth = regisManager->get_colorMethode();
    Collection* cloud_P = sceneManager->get_selected_collection();
    Collection* cloud_Q = sceneManager->get_cloud_next();
    //---------------------------

    if(ImGui::RadioButton("None", colorMeth, 0)){
      regisManager->colorization(cloud_P, cloud_Q);
    }

    if(ImGui::RadioButton("Red Green", colorMeth, 1)){
      regisManager->colorization(cloud_P, cloud_Q);
    }

    if(ImGui::RadioButton("Red Green -", colorMeth, 2)){
      regisManager->colorization(cloud_P, cloud_Q);
    }

    if(ImGui::RadioButton("Transparent", colorMeth, 3)){
      regisManager->colorization(cloud_P, cloud_Q);
    }

    //---------------------------
    ImGui::Separator();
  }*/
}
void GUI_registration::regist_DOF(){
  if(ImGui::CollapsingHeader("DoF")){
    //---------------------------

    //Degree of freedoms - Rotation
    static bool Rx = true;
    static bool Ry = true;
    static bool Rz = true;
    if(ImGui::Checkbox("Rx", &Rx)){
      icpManager->set_dof_rotation(Rx, Ry, Rz);
    }
    ImGui::SameLine();
    if(ImGui::Checkbox("Ry", &Ry)){
      icpManager->set_dof_rotation(Rx, Ry, Rz);
    }
    ImGui::SameLine();
    if(ImGui::Checkbox("Rz", &Rz)){
      icpManager->set_dof_rotation(Rx, Ry, Rz);
    }
    static float Rx_weight = 1;
    static float Ry_weight = 1;
    static float Rz_weight = 1;
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##555", &Rx_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_rot_weight(vec3(Rx_weight, Ry_weight, Rz_weight));
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##556", &Ry_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_rot_weight(vec3(Rx_weight, Ry_weight, Rz_weight));
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##557", &Rz_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_rot_weight(vec3(Rx_weight, Ry_weight, Rz_weight));
    }

    //Degree of freedoms - Translation
    static bool tx = true;
    static bool ty = true;
    static bool tz = true;
    if(ImGui::Checkbox("tx", &tx)){
      icpManager->set_dof_translation(tx, ty, tz);
    }
    ImGui::SameLine();
    if(ImGui::Checkbox("ty", &ty)){
      icpManager->set_dof_translation(tx, ty, tz);
    }
    ImGui::SameLine();
    if(ImGui::Checkbox("tz", &tz)){
      icpManager->set_dof_translation(tx, ty, tz);
    }
    static float Tx_weight = 1;
    static float Ty_weight = 1;
    static float Tz_weight = 1;
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##558", &Tx_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_tra_weight(vec3(Tx_weight, Ty_weight, Tz_weight));
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##559", &Ty_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_tra_weight(vec3(Tx_weight, Ty_weight, Tz_weight));
    }
    ImGui::SameLine();
    ImGui::PushItemWidth(40);
    if(ImGui::DragFloat("##560", &Tz_weight, 0.01f, 0, 1, "%.2f")){
      icpManager->set_dof_tra_weight(vec3(Tx_weight, Ty_weight, Tz_weight));
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_registration::regist_Parameters(){
  if(ImGui::CollapsingHeader("Parameters")){
    //---------------------------

    //Choose registration algo
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Pipeline");
    const char* cstch = "Custom\0ICP PCL\0NDT PCL\0GICP PCL\0LUM PCL\0pts4congruents PCL\0";
    ImGui::SetNextItemWidth(150);
    ImGui::Combo("##1", &regis_algo, cstch);

    //Choose optimization algo
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Optimization");
    int* optim_algo = icpManager->get_optimizationMethod();
    const char* optim_choices = "Newton-Raphson\0SVD PCL\0LM PCL\0Dual quaternion PCL\0None\0";
    ImGui::SetNextItemWidth(150);
    ImGui::Combo("##2", optim_algo, optim_choices);

    static float icpGain = 1.0f;
    ImGui::PushItemWidth(100);
    if(ImGui::DragFloat("Gain", &icpGain, 0.01f, 0, 10, "%.2f")){
      icpManager->set_icpGain(icpGain);
    }

    //Choose center of mass - root or COM
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "Rotation centering");
    if(regis_algo == 0){
      int* com = icpManager->get_optimCOM();
      ImGui::RadioButton("COM", com, 0);
      ImGui::SameLine();
      ImGui::RadioButton("Root", com, 1);
    }

    //End loop conditions
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f), "End loop conditions");
    int* nbIter = regisManager->get_nbIter_max();
    ImGui::PushItemWidth(100);
    ImGui::InputInt("max iter", nbIter);

    float* SSE_max = regisManager->get_SSE_max();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("max SSE", SSE_max, 0.001f, 0, 1.0f, "%.4f");

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_registration::regist_Registration(){
  //---------------------------

  //Make an iteration
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  static bool ICP_continu = false;
  if(ImGui::Button("Iteration+1", ImVec2(150,0)) || ICP_continu){
    if(sceneManager->get_nb_cloud() > 1){
      switch (regis_algo){
        case 0: regisManager->make_Iteration(); break;
      }
    }
  }
  ImGui::PopStyleColor(1);

  //Start / stop continuous iterations
  if(ImGui::Button("start", ImVec2(70,0))){
    ICP_continu = true;
  }
  ImGui::SameLine();
  if(ImGui::Button("stop", ImVec2(70,0))){
    regisManager->restart();
    icpManager->reset();
    ICP_continu = false;
  }

  //Print error minimization
  if(ImGui::Button("Print", ImVec2(70,0))){
    regisManager->print_ICP();
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_registration::regist_Stats(){
  /*Collection* collection = sceneManager->get_selected_collection();
  Collection* cloud_Q = sceneManager->get_cloud_next();
  //---------------------------

  //DATA
  //--->translation
  vec3 Xt = icpManager->get_translat();
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Xt : %.4f %.4f %.4f", Xt.x, Xt.y, Xt.z);
  vec3 Xr = icpManager->get_rotation();
  Xr.x = Xr.x *180/M_PI;
  Xr.y = Xr.y *180/M_PI;
  Xr.z = Xr.z *180/M_PI;
  ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Xr(°) : %.4f %.4f %.4f", Xr.x, Xr.y, Xr.z);

  //--->world transformation
  vec3 E_trans, E_angle;
  if(!sceneManager->get_is_list_empty()){
    E_trans = poseManager.compute_translationsError(cloud);
    E_angle = poseManager.compute_anglesError(cloud);

    E_trans.x = E_trans.x * 1000;
    E_trans.y = E_trans.y * 1000;
    E_trans.z = E_trans.z * 1000;

    E_angle.x = E_angle.x * (180.0/3.141592653589793238463);
    E_angle.y = E_angle.y * (180.0/3.141592653589793238463);
    E_angle.z = E_angle.z * (180.0/3.141592653589793238463);

    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Et (mm) : %.1f %.1f %.1f", E_trans.x, E_trans.y, E_trans.z);
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Er (°) : %.3f %.3f %.3f", E_angle.x, E_angle.y, E_angle.z);
    ImGui::SameLine();
    if(ImGui::TreeNode("##146") && !sceneManager->get_is_list_empty()){
      mat4 matReal = cloud->transformation.RealTransformation;
      mat4 matIcp = cloud->transformation.TransformationMatrix;
      vec3 angleReal = poseManager.compute_anglesFromTransformationMatrix(matReal);
      vec3 transReal = poseManager.compute_translFromTransformationMatrix(matReal);
      vec3 angleIcp = poseManager.compute_anglesFromTransformationMatrix(matIcp);
      vec3 transIcp = transformManager->compute_translFromTransformationMatrix(matIcp);
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Real");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "Tw : %.3f %.3f %.3f", transReal.x, transReal.y, transReal.z);
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"ICP ");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "Tw : %.3f %.3f %.3f", transIcp.x, transIcp.y, transIcp.z);
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Real");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "Rw : %.3f %.3f %.3f", angleReal.x, angleReal.y, angleReal.z);
      ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"ICP ");
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "Rw : %.3f %.3f %.3f", angleIcp.x, angleIcp.y, angleIcp.z);
    }

    //--->Iteration
    int iter = regisManager->get_iter();
    int* iter_max = regisManager->get_nbIter_max();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Iter : %d", iter);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "/%d", *iter_max);

    //--->Errors
    float SSE = icpManager->compute_SSE(cloud, cloud_Q);
    float* SSE_max = regisManager->get_SSE_max();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "SSE : %f", SSE);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "/%.3f", *SSE_max);
    float MSE_groundTruth = icpManager->compute_MSE_groundTruth(cloud);
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "MSE ground truth : %f", MSE_groundTruth);
    float RMSE_groundTruth = icpManager->compute_RMSE_groundTruth(cloud);
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "RMSE ground truth : %f", RMSE_groundTruth);
    float MAE_groundTruth = icpManager->compute_MAE_groundTruth(cloud);
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "MAE ground truth : %f", MAE_groundTruth);

    //--->Time execution
    float ICP_duration = regisManager->get_duration();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "ICP : %f", ICP_duration);
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "ms");
    ImGui::SameLine();

    //--->Time execution more precise
    if(ImGui::TreeNode("##147")){
      ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "   Matching : %f", icpManager->get_time_matching());
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "ms");
      ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "   Rejection : %f", icpManager->get_time_rejection());
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "ms");
      ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "   Optimization : %f", icpManager->get_time_optimization());
      ImGui::SameLine();
      ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "ms");
    }
  }*/

  //---------------------------
}
