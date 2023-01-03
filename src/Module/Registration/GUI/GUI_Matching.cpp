#include "GUI_Matching.h"

#include "../Module_registration.h"
#include "../src/Registration.h"
#include "../src/Pipeline/ICP.h"
#include "../src/Correspondence/Keypoint.h"

#include "../../Radiometry/src/Radiometry.h"
#include "../../Radiometry/src/Approach/Linearization.h"

#include "../../../GUI/Modal/GUI_Modal.h"
#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Attribut.h"


//Constructor / Destructor
GUI_matching::GUI_matching(Module_registration* module){
  //---------------------------

  Node_engine* node_engine = module->get_node_engine();
  Node_module* node_module = module->get_node_module();
  Node_operation* node_ope = module->get_node_ope();
  //Registration* regisManager = module->get_regisManager();

  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->attribManager = node_ope->get_attribManager();
  //this->icpManager = regisManager->get_icpManager();
  //this->keyManager = icpManager->get_keyManager();
  //this->radioManager = node_module->get_radioManager();

  //---------------------------
}
GUI_matching::~GUI_matching(){}

//Main function
void GUI_matching::design_Matching(){
  //---------------------------

  this->match_matching();
  this->match_keypoint();
  this->match_dense();
  this->match_methods();

  //---------------------------
}

//Subcategories
void GUI_matching::match_matching(){
  Cloud* cloud_P = sceneManager->get_selected_cloud();
  Cloud* cloud_Q = sceneManager->get_cloud_next();
  //---------------------------

  //Matching method
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Matching");
  ImGui::PushItemWidth(175);

  int* meth_matching = icpManager->get_matchingMethod();
  const char* cstch = "Adaptative (XYZI/XYZ)\0Keypoint\0Intensity (XYZI)\0Geometric (XYZ)\0Pure intensity (I)\0UserSelection\0directMatching\0ICCP\0";
  ImGui::Combo("##1", meth_matching, cstch);

  //Apply matching
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Apply", ImVec2(175,0))){
    int nbCloud = sceneManager->get_nb_cloud();
    if(nbCloud >=2){
      Cloud* cloud_P = sceneManager->get_selected_cloud();
      Cloud* cloud_Q = sceneManager->get_cloud_next();

      bool* restartCorrespondences = icpManager->get_makeCorrespondences();
      *restartCorrespondences = true;
      icpManager->compute_prepareData(cloud_P, cloud_Q);
      icpManager->step_matching(cloud_P, cloud_Q);
      icpManager->step_rejection(cloud_P, cloud_Q);

      //glyphManager->obj_matching(cloud_P, cloud_Q);
    }else if(nbCloud == 1 && *meth_matching == 1){
      //keyManager->algo_keypoints_one(cloud_P);
    }
  }
  ImGui::PopStyleColor(1);

  //Correspondance displaying
  /*bool* matchingON = glyphManager->get_matchVisibility();
  ImGui::Checkbox("Display", matchingON);
  ImGui::SameLine();
  static bool matchingRDM = false;
  if(ImGui::Checkbox("Rdm color", &matchingRDM)){
    glyphManager->set_matching_rdm_color(matchingRDM);
  }*/

  //---------------------------
  ImGui::Separator();
}
void GUI_matching::match_keypoint(){
  int* meth_matching = icpManager->get_matchingMethod();
  if(*meth_matching == 1){
    //--------------------------

    this->keypoint_options();
    this->keypoint_parameters();
    this->keypoint_rejection();

    //--------------------------
    ImGui::Separator();
  }
}
void GUI_matching::match_dense(){
  int* meth_matching = icpManager->get_matchingMethod();
  if(*meth_matching == 0 || *meth_matching == 2 || *meth_matching == 3){
    //--------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Dense");

    //Percentage of point taking into account
    static int percentagePts = 100;
    ImGui::PushItemWidth(100);
    if(ImGui::DragInt("% points", &percentagePts, 1, 1, 100, "%d")){
      icpManager->set_matchingPercentPts((int)percentagePts);
    }

    //--------------------------
    ImGui::Separator();
  }
}
void GUI_matching::match_methods(){
  Cloud* cloud_P = sceneManager->get_selected_cloud();
  Cloud* cloud_Q = sceneManager->get_cloud_next();
  //---------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Methods");

  //Rejection options
  if(ImGui::CollapsingHeader("Rejection")){
    //Rejection based on the maximum allowed distance
    bool* rejection_distance = icpManager->get_rejectDistance();
    ImGui::Checkbox("Rejection distance", rejection_distance);
    if(*rejection_distance){
      float* rejection_distance_threshold = icpManager->get_rejectDistance_threshold();
      ImGui::DragFloat("Threshold", rejection_distance_threshold, 0.001, 0.0f, 20.0f, "%.3f");
    }

    //Rejection based on the maximum difference of normal
    static bool rejectNormal = false;
    if(ImGui::Checkbox("Min normal", &rejectNormal)){
        icpManager->set_rejectNormal(rejectNormal);
    }

    //Rejection of the two or more pair of correspondence
    static bool rejectDuplicata = false;
    if(ImGui::Checkbox("Duplicata", &rejectDuplicata)){
        icpManager->set_rejectDuplicata(rejectDuplicata);
    }
  }

  //SIFT parameters
  /*if(ImGui::CollapsingHeader("SIFT##008")){
    float* SIFT_minScale = keyManager->get_SIFT_minScale();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("min scale", SIFT_minScale, 0.001, 0.001f, 10.0f, "%.5f");

    float* SIFT_minContrast = keyManager->get_SIFT_minContrast();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("min contrast", SIFT_minContrast, 0.1, 0.0f, 20.0f, "%.5f");

    int* SIFT_nbOctaves = keyManager->get_SIFT_nbOctaves();
    ImGui::PushItemWidth(100);
    ImGui::DragInt("nb octaves", SIFT_nbOctaves, 1, 0, 100, "%d");

    int* SIFT_nbScalePerOctave = keyManager->get_SIFT_nbScalePerOctave();
    ImGui::PushItemWidth(100);
    ImGui::DragInt("nb scale per octave", SIFT_nbScalePerOctave, 1, 0, 100, "%d");

    ImGui::Separator();
  }

  //Harris 3D options
  if(ImGui::CollapsingHeader("HARRIS 3D##009")){
    float* HARRIS_thres = keyManager->get_HARRIS_threshold();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("Threshold", HARRIS_thres, 0.000001, 0.0000001f, 10.0f, "%.7f");

    ImGui::Separator();
  }*/

  //---------------------------
}

//Subfunctions
void GUI_matching::keypoint_options(){
  /*ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Options");
  int* dataMeth = keyManager->get_dataMethod();
  int* downMeth = keyManager->get_downMethod();
  int* normMeth = keyManager->get_normalMethod();
  int* keyMeth = keyManager->get_keypMethod();
  int* desMeth = keyManager->get_descMethod();
  //--------------------------

  //Colorimetry
  ImGui::RadioButton("RGB ", dataMeth, 0);
  ImGui::SameLine();
  ImGui::RadioButton("I", dataMeth, 1);

  //Downsampling
  ImGui::RadioButton("None##001", downMeth, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Voxel", downMeth, 1);
  ImGui::SameLine();
  ImGui::RadioButton("Segme", downMeth, 2);

  //Normals
  ImGui::RadioButton("None", normMeth, 0);
  ImGui::SameLine();
  ImGui::RadioButton("Nxyz ", normMeth, 1);

  //Keypoints
  ImGui::RadioButton("None##002", keyMeth, 0);
  ImGui::SameLine();
  ImGui::RadioButton("SIFT ", keyMeth, 1);
  ImGui::SameLine();
  ImGui::RadioButton("HARRIS", keyMeth, 2);
  ImGui::SameLine();
  ImGui::RadioButton("SUSAN", keyMeth, 3);

  //Descriptors
  ImGui::RadioButton("None##003", desMeth, 0);
  ImGui::SameLine();
  ImGui::RadioButton("SHOT ", desMeth, 1);
  ImGui::SameLine();
  ImGui::RadioButton("FPFH", desMeth, 2);

  //--------------------------*/
}
void GUI_matching::keypoint_parameters(){
  /*int* downMeth = keyManager->get_downMethod();
  int* keyMeth = keyManager->get_keypMethod();
  int* desMeth = keyManager->get_descMethod();
  //--------------------------

  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Parameters");

  //Voxel options
  if(*downMeth == 1){
    ImGui::SetNextItemWidth(100);
    float* voxeleaf = keyManager->get_Down_VoxelLeaf();
    ImGui::DragFloat("Voxel: leaf size", voxeleaf, 0.001, 0.00001f, 10.0f, "%.3f");
  }

  //SIFT options
  if(*keyMeth == 1){
    bool* showSphere = keyManager->get_showSphereAtKeypoint();
    ImGui::Checkbox("SIFT: show keypoints", showSphere);

    float* SIFT_minContrast = keyManager->get_SIFT_minContrast();
    ImGui::PushItemWidth(100);
    ImGui::DragFloat("SIFT: min contrast", SIFT_minContrast, 0.1, 0.0f, 20.0f, "%.5f / 255");

    ImGui::SetNextItemWidth(100);
    float* keyRadius = keyManager->get_keypoint_radius();
    ImGui::DragFloat("SIFT: radius", keyRadius, 0.001, 0.0f, 10.0f, "%.3f");
  }

  //Descriptor options
  if(*desMeth == 1 || *desMeth == 2){
    ImGui::SetNextItemWidth(100);
    float* descRadius = keyManager->get_descriptor_radius();
    ImGui::DragFloat("Descriptor: radius", descRadius, 0.001, 0.001, 10.0f, "%.3f");
  }

  //--------------------------*/
}
void GUI_matching::keypoint_rejection(){
  /*//--------------------------

  //SIFT min distance between two keypoints
  int* keyMeth = keyManager->get_keypMethod();
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Rejection");
  if(*keyMeth == 1){
    bool* siftNN = keyManager->get_rejectSiftRadiusLimit();
    ImGui::Checkbox("SIFT: radius limit", siftNN);
  }

  //Correspondances
  bool* correspScore = keyManager->get_rejectCorrespSCORE();
  ImGui::Checkbox("Corresp: score", correspScore);

  bool* correspNbBest = keyManager->get_rejectCorrespNbBestON();
  ImGui::Checkbox("Corresp: nb best##2", correspNbBest);
  if(*correspNbBest == true){
    int* nbBestCorresp = keyManager->get_rejectCorrespNbBest();
    ImGui::SetNextItemWidth(100);
    ImGui::DragInt("Number of best", nbBestCorresp, 1, 1, 10000, "%d");
  }

  //RANSAC
  bool* correspRansac = keyManager->get_rejectCorrespRANSAC();
  ImGui::Checkbox("RANSAC: rejection", correspRansac);
  if(*correspRansac == true){
    int* ransacIterMax = keyManager->get_rejectRANSAC_iterMax();
    ImGui::SetNextItemWidth(100);
    ImGui::DragInt("Iteration max", ransacIterMax, 100, 1, 100000, "%d");
  }

  //--------------------------*/
}
