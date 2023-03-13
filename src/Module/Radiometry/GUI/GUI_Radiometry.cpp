#include "GUI_Radiometry.h"

#include "../Module_radiometry.h"
#include "../src/Radiometry.h"
#include "../src/Plot_radio.h"
#include "../src/Target/Reference.h"
#include "../src/Approach/Linearization.h"
#include "../src/Approach/Surfacic_local.h"
#include "../src/Approach/Surfacic_global_piecewise.h"
#include "../src/Approach/Surfacic_simplified.h"
#include "../src/Approach/Surfacic_segmented.h"
#include "../src/Approach/Separation_global.h"

#include "../../../GUI/Node_gui.h"
#include "../../../GUI/Modal/GUI_Modal.h"
#include "../../../Engine/Node_engine.h"
#include "../../../Scene/Node_scene.h"
#include "../../../Scene/Data/Scene.h"
#include "../../../Load/Node_load.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Transformation/Attribut.h"
#include "../../../Operation/Color/Heatmap.h"
#include "../../../Operation/Function/Plotting.h"
#include "../../../Operation/Function/BundleByClass.h"
#include "../../../Specific/File/Zenity.h"


//Constructor / Destructor
GUI_radiometry::GUI_radiometry(Module_radiometry* module){
  //---------------------------

  Node_engine* node_engine = module->get_node_engine();
  Node_module* node_module = module->get_node_module();
  Node_operation* node_ope = module->get_node_ope();
  Node_load* node_load = module->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  //this->radioManager = module->get_radioManager();
  this->refManager = radioManager->get_refManager();
  this->linManager = radioManager->get_linManager();
  this->radioplotManager = radioManager->get_plot_radio();

  this->corr_data = 0;
  this->corr_num = 1;
  this->corr_ref = true;
  this->corr_heat = true;

  //---------------------------
}
GUI_radiometry::~GUI_radiometry(){}

//Main function
void GUI_radiometry::design_Radiometry(){
  //---------------------------

  this->cloudStat();
  this->correction();
  this->plotting();
  this->calibrationTargets();
  this->options();
  this->Approaches();

  //---------------------------
}

//Subfunctions
void GUI_radiometry::cloudStat(){
  /*Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //Radiometric cloud state
  if(!sceneManager->get_is_list_empty()){
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "No cloud");
  }else if(cloud->intensity.corrected == false){
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Cloud");ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f), "uncorrected");
  }else if(cloud->intensity.corrected == true && cloud->intensity.linearized == false){
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Cloud");ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,1.0f,1.0f), "corrected");
  }else if(cloud->intensity.linearized == true){
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Cloud");ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f,1.0f,0.0f,1.0f), "linearized");
  }
  //---------------------------
  ImGui::Separator();*/
}
void GUI_radiometry::correction(){
  Collection* collection = sceneManager->get_selected_collection();
  //---------------------------

  //Intensity correction
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Correction");

  int* algoCorr = radioManager->get_algoCorrection();
  ImGui::PushItemWidth(207.5);
  ImGui::Combo("##10", algoCorr, "Surfacic segmented\0Surfacic global piecewise\0Separation global\0Surfacic local\0Surfacic simplified\0Radar equ.\0");
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Apply##correction", ImVec2(100,0))){
    //Radiometry correction ptions
    if(!sceneManager->get_is_list_empty()){
      if(corr_num == 1 && corr_ref && corr_data == 0){
        radioManager->correction_allClouds();
      }else if(corr_num == 1 && !corr_ref && corr_data == 0){
        radioManager->correction_allClouds_WtRefs();
      }else if(corr_num == 0 && corr_data == 0){
        radioManager->correction_oneCloud(cloud);
      }else if(corr_num == 1 && corr_data == 1){
        radioManager->correction_allClouds_Iini();
      }else if(corr_num == 0 && corr_data == 1){
        radioManager->correction_oneCloud_Iini(cloud);
      }
    }
  }
  ImGui::PopStyleColor(1);
  ImGui::SameLine();
  if(ImGui::Button("Reverse##correction", ImVec2(100,0))){
    if(!sceneManager->get_is_list_empty()){
      sceneManager->reset_collection(collection);
    }
  }

  //Intensity linearization
  ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Linearization");

  ImGui::PushItemWidth(207.5);
  static int method = 0;
  ImGui::Combo("##11", &method, "Surfacic global piecewise\0Separation global\0Surfacic local\0");
  ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(46, 75, 133, 255));
  if(ImGui::Button("Apply##linearization", ImVec2(100,0))){
    if(corr_num == 1){
      list<Collection*>* list_collection = sceneManager->get_list_col_object();
      //-------------------------------

      for(int i=0; i<list_collection->size(); i++){
        Collection* collection = *next(list_collection->begin(),i);
        linManager->algo_linearization(cloud, method);
      }

      //-------------------------------
    }else{
      linManager->algo_linearization(cloud, method);
    }
  }
  ImGui::PopStyleColor(1);
  ImGui::SameLine();
  if(ImGui::Button("Reverse##linearization", ImVec2(100,0))){
    if(corr_num == 1){
      list<Collection*>* list_collection = sceneManager->get_list_col_object();
      //-------------------------------

      for(int i=0; i<list_collection->size(); i++){
        Collection* collection = *next(list_collection->begin(),i);
        linManager->algo_reverse(cloud, method);
      }

      //-------------------------------
    }else{
      linManager->algo_reverse(cloud, method);
    }
  }

  //---------------------------
  ImGui::Separator();
}
void GUI_radiometry::plotting(){
  if(ImGui::CollapsingHeader("Plotting")){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    const char* name_c = cloud->name.c_str();
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity");
    Plotting plotManager;
    //---------------------------

    if(ImGui::Button("Histogram", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        plotManager.set_Xlabel("Intensity");
        plotManager.set_Ylabel("Number of points");
        plotManager.plot_histogram(cloud->I);
      }
    }

    if(ImGui::Button("I(R)", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioplotManager->plot_IbyR_data(cloud);
      }
    }

    if(ImGui::Button("I(It)", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioplotManager->plot_IbyIt_cloud(cloud);
      }
    }

    if(ImGui::Button("I(R) all", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioplotManager->plot_IbyR();
      }
    }

    if(ImGui::Button("I(It) all", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioplotManager->plot_IbyCosIt_all();
      }
    }

    if(ImGui::Button("I(It,R)", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioplotManager->plot_IbyItbyR();
      }
    }

    /*if(ImGui::Button("Special", ImVec2(150,0))){
      list<Collection*>* list = sceneManager->get_list_col_object();
      vector<float> Is_99, Is_50, Is_25, Is_10;
      vector<float> Istd_99, Istd_50, Istd_25, Istd_10;
      vector<float> R_99, R_50, R_25, R_10;
      vector<float> It_99, It_50, It_25, It_10;
      Attribut attribManager(sceneManager);
      BundleByClass* bundler = new BundleByClass();
      //---------------------------

      for(int i=0; i<list->size(); i++){
        Collection* collection = *next(list->begin(),i);
        attribManager.compute_attribut_cloud(cloud);

        if(cloud->name.find("10p") != std::string::npos){
          //Bundle by classes
          bundler->compute_bundleByClass(cloud, 2);
          vector<float> Ib = bundler->get_Ib();
          vector<float> Ib_std = bundler->get_Ib_std();
          vector<float> Ib_R = bundler->get_Ib_dist();
          vector<float> Ib_It = bundler->get_Ib_It();

          for(int j=0; j<Ib.size(); j++){
            Is_10.push_back(Ib[j]);
            Istd_10.push_back(Ib_std[j]);
            R_10.push_back(Ib_R[j]);
            It_10.push_back(Ib_It[j]);
          }
        }
        if(cloud->name.find("25p") != std::string::npos){
          //Bundle by classes
          bundler->compute_bundleByClass(cloud, 2);
          vector<float> Ib = bundler->get_Ib();
          vector<float> Ib_std = bundler->get_Ib_std();
          vector<float> Ib_R = bundler->get_Ib_dist();
          vector<float> Ib_It = bundler->get_Ib_It();

          for(int j=0; j<Ib.size(); j++){
            Is_25.push_back(Ib[j]);
            Istd_25.push_back(Ib_std[j]);
            R_25.push_back(Ib_R[j]);
            It_25.push_back(Ib_It[j]);
          }
        }
        if(cloud->name.find("50p") != std::string::npos){
          //Bundle by classes
          bundler->compute_bundleByClass(cloud, 2);
          vector<float> Ib = bundler->get_Ib();
          vector<float> Ib_std = bundler->get_Ib_std();
          vector<float> Ib_R = bundler->get_Ib_dist();
          vector<float> Ib_It = bundler->get_Ib_It();

          for(int j=0; j<Ib.size(); j++){
            Is_50.push_back(Ib[j]);
            Istd_50.push_back(Ib_std[j]);
            R_50.push_back(Ib_R[j]);
            It_50.push_back(Ib_It[j]);
          }
        }
        if(cloud->name.find("99p") != std::string::npos){
          //Bundle by classes
          bundler->compute_bundleByClass(cloud, 2);
          vector<float> Ib = bundler->get_Ib();
          vector<float> Ib_std = bundler->get_Ib_std();
          vector<float> Ib_R = bundler->get_Ib_dist();
          vector<float> Ib_It = bundler->get_Ib_It();

          for(int j=0; j<Ib.size(); j++){
            Is_99.push_back(Ib[j]);
            Istd_99.push_back(Ib_std[j]);
            R_99.push_back(Ib_R[j]);
            It_99.push_back(Ib_It[j]);
          }
        }
      }

      //=========================================================

      vector<float> D0, D1, D2;

      //99%
      if(true){
        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(R_99)) {
          D0.push_back(Is_99[i]);
          D1.push_back(Istd_99[i]);
          D2.push_back(R_99[i]);
        }

        //Write data on file
        ofstream file_99_R;
        file_99_R.open ("../../media/data/data/Spectralon_random_IbyR_99.txt");
        file_99_R << "#99% -> Is Istd R \n";
        file_99_R << "#CV = "<<fct_cv(Is_99)<< "\n";
        file_99_R << "#--------------------------------"<<"\n";
        file_99_R <<std::fixed;
        for(int i=0; i<Is_99.size(); i++){
          file_99_R <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_99_R.close();

        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(It_99)) {
          D0.push_back(Is_99[i]);
          D1.push_back(Istd_99[i]);
          D2.push_back(It_99[i]);
        }

        ofstream file_99_It;
        file_99_It.open ("../../media/data/data/Spectralon_random_IbyIt_99.txt");
        file_99_It << "#99% -> Is Istd R It \n";
        file_99_It << "#CV = "<<fct_cv(Is_99)<< "\n";
        file_99_It << "#--------------------------------"<<"\n";
        file_99_It <<std::fixed;
        for(int i=0; i<Is_99.size(); i++){
          file_99_It <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_99_It.close();
      }

      //=========================================================

      //50%
      if(true){
        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(R_50)) {
          D0.push_back(Is_50[i]);
          D1.push_back(Istd_50[i]);
          D2.push_back(R_50[i]);
        }

        //Write data on file
        ofstream file_50_R;
        file_50_R.open ("../../media/data/data/Spectralon_random_IbyR_50.txt");
        file_50_R << "#99% -> Is Istd R \n";
        file_50_R << "#CV = "<<fct_cv(Is_50)<< "\n";
        file_50_R << "#--------------------------------"<<"\n";
        file_50_R <<std::fixed;
        for(int i=0; i<Is_50.size(); i++){
          file_50_R <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_50_R.close();

        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(It_50)) {
          D0.push_back(Is_50[i]);
          D1.push_back(Istd_50[i]);
          D2.push_back(It_50[i]);
        }

        ofstream file_50_It;
        file_50_It.open ("../../media/data/data/Spectralon_random_IbyIt_50.txt");
        file_50_It << "#99% -> Is Istd R It \n";
        file_50_It << "#CV = "<<fct_cv(Is_50)<< "\n";
        file_50_It << "#--------------------------------"<<"\n";
        file_50_It <<std::fixed;
        for(int i=0; i<Is_50.size(); i++){
          file_50_It <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_50_It.close();
      }

      //=========================================================

      //25%
      if(true){
        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(R_25)) {
          D0.push_back(Is_25[i]);
          D1.push_back(Istd_25[i]);
          D2.push_back(R_25[i]);
        }

        //Write data on file
        ofstream file_25_R;
        file_25_R.open ("../../media/data/data/Spectralon_random_IbyR_25.txt");
        file_25_R << "#99% -> Is Istd R \n";
        file_25_R << "#CV = "<<fct_cv(Is_25)<< "\n";
        file_25_R << "#--------------------------------"<<"\n";
        file_25_R <<std::fixed;
        for(int i=0; i<Is_25.size(); i++){
          file_25_R <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_25_R.close();

        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(It_25)) {
          D0.push_back(Is_25[i]);
          D1.push_back(Istd_25[i]);
          D2.push_back(It_25[i]);
        }

        ofstream file_25_It;
        file_25_It.open ("../../media/data/data/Spectralon_random_IbyIt_25.txt");
        file_25_It << "#99% -> Is Istd R It \n";
        file_25_It << "#CV = "<<fct_cv(Is_25)<< "\n";
        file_25_It << "#--------------------------------"<<"\n";
        file_25_It <<std::fixed;
        for(int i=0; i<Is_25.size(); i++){
          file_25_It <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_25_It.close();
      }

      //=========================================================

      //10%
      if(true){
        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(R_10)) {
          D0.push_back(Is_10[i]);
          D1.push_back(Istd_10[i]);
          D2.push_back(R_10[i]);
        }

        //Write data on file
        ofstream file_10_R;
        file_10_R.open ("../../media/data/data/Spectralon_random_IbyR_10.txt");
        file_10_R << "#99% -> Is Istd R \n";
        file_10_R << "#CV = "<<fct_cv(Is_10)<< "\n";
        file_10_R << "#--------------------------------"<<"\n";
        file_10_R <<std::fixed;
        for(int i=0; i<Is_10.size(); i++){
          file_10_R <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_10_R.close();

        D0.clear(); D1.clear(); D2.clear();
        for (auto i: fct_sortByIndexes(It_10)) {
          D0.push_back(Is_10[i]);
          D1.push_back(Istd_10[i]);
          D2.push_back(It_10[i]);
        }

        ofstream file_10_It;
        file_10_It.open ("../../media/data/data/Spectralon_random_IbyIt_10.txt");
        file_10_It << "#99% -> Is Istd R It \n";
        file_10_It << "#CV = "<<fct_cv(Is_10)<< "\n";
        file_10_It << "#--------------------------------"<<"\n";
        file_10_It <<std::fixed;
        for(int i=0; i<Is_10.size(); i++){
          file_10_It <<setprecision(5) << D0[i] << " " << D1[i] << " " << D2[i] << "\n";
        }
        file_10_It.close();
      }

      //---------------------------
    }*/

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_radiometry::calibrationTargets(){
  if(ImGui::CollapsingHeader("Calibration targets")){
    //---------------------------

    //General
    ImGui::TextColored(ImVec4(0.0f,0.75f,0.0f,1.0f), "Main folder");
    ImGui::SameLine();
    string* path_calibrationTargets = refManager->get_path_calibrationTargets();
    if(ImGui::Button("...##0")){
      zenity_directory(*path_calibrationTargets);
    }

    //Sphere
    ImGui::TextColored(ImVec4(0.0f,0.75f,0.0f,1.0f), "Sphere");
    ImGui::SameLine();
    string* path_sphereTarget_add80d = refManager->get_path_sphereTarget_add80d();
    if(ImGui::Button("...##sphere")){
      zenity_directory(*path_sphereTarget_add80d);
    }
    ImGui::SameLine();
    if(ImGui::Button("I(R)##sphere", ImVec2(60,0))){
      refManager->load_References_path(*path_sphereTarget_add80d);
      radioplotManager->plot_IbyR();
      refManager->clear();
    }
    ImGui::SameLine();
    if(ImGui::Button("I(alpha)##sphere", ImVec2(60,0))){
      refManager->load_References_path(*path_sphereTarget_add80d);
      Collection* collection = refManager->get_specificSphere("10.0m");
      say(cloud->name);
      radioplotManager->plot_IbyIt_cloud(cloud);
      refManager->clear();
    }
    ImGui::SameLine();
    if(ImGui::Button("I(R,alpha)##sphere", ImVec2(60,0))){
      refManager->load_References_path(*path_sphereTarget_add80d);
      radioplotManager->plot_ParameterSpace();
      refManager->clear();
    }

    //Spectralon - Range
    ImGui::TextColored(ImVec4(0.0f,0.75f,0.0f,1.0f), "Spectralon - range");
    ImGui::SameLine();
    string* path_spectralonTarget_dist = refManager->get_path_spectralonTarget_dist();
    if(ImGui::Button("...##spectralon1")){
      zenity_directory(*path_spectralonTarget_dist);
    }
    ImGui::SameLine();
    if(ImGui::Button("I(R)##spectralon", ImVec2(60,0))){
      refManager->load_References_path(*path_spectralonTarget_dist);
      radioplotManager->plot_IbyR();
      refManager->clear();
    }

    //Spectralon - Angle
    ImGui::TextColored(ImVec4(0.0f,0.75f,0.0f,1.0f), "Spectralon - angle");
    ImGui::SameLine();
    string* path_spectralonTarget_angle = refManager->get_path_spectralonTarget_angle();
    if(ImGui::Button("...##spectralon2")){
      zenity_directory(*path_spectralonTarget_angle);
    }
    ImGui::SameLine();
    if(ImGui::Button("I(alpha)##spectralon", ImVec2(60,0))){
      refManager->load_References_path(*path_spectralonTarget_angle);
      radioplotManager->plot_IbyCosIt_all();
      refManager->clear();
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_radiometry::options(){
  if(ImGui::CollapsingHeader("Options")){
    //---------------------------

    //If correction on obj or initial cloud location
    ImGui::RadioButton("Ini", &corr_data, 1); ImGui::SameLine();
    ImGui::RadioButton("OBJ", &corr_data, 0);

    //Selection of clouds to correct
    ImGui::RadioButton("One", &corr_num, 0); ImGui::SameLine();
    ImGui::RadioButton("All", &corr_num, 1);

    //If references are to be corrected (?)
    ImGui::Checkbox("Refs", &corr_ref); ImGui::SameLine();

    //If apply heatmap on processed cloud
    ImGui::Checkbox("Heatmap", &corr_heat);

    //Reference options
    if(ImGui::Button("Remove references", ImVec2(150,0))){
      if(!sceneManager->get_is_list_empty()){
        radioManager->remove_References();
      }
    }
    static bool referenceON = true;
    if(ImGui::Button("Visibility references", ImVec2(150,0))){
      referenceON = !referenceON;
      radioManager->set_referenceON(referenceON);
    }

    //---------------------------
    ImGui::Separator();
  }
}
void GUI_radiometry::Approaches(){
  /*if(ImGui::CollapsingHeader("Approach")){
    Collection* collection = sceneManager->get_selected_collection();
    //---------------------------

    //Surfacic global piecewise approach
    if(ImGui::TreeNode("Surfacic global piecewise")){
      Surfacic_global_piecewise* surf_globalManager = radioManager->get_Surfacic_global_piecewise();
      ImGui::Separator();
      //---------------------------

      //Plotting
      if(ImGui::Button("plot fitted surface", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        surf_globalManager->plot_SurfaceFitting();
      }
      if(ImGui::Button("plot space parameter", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        surf_globalManager->plot_SpaceParameter();
      }

      //Polynomial regression degrees
      static int m = 2;
      static int n = 2;
      ImGui::PushItemWidth(100);
      if(ImGui::InputInt("m", &m)){
        surf_globalManager->set_m(m);
      }
      ImGui::PushItemWidth(100);
      if(ImGui::InputInt("n", &n)){
        surf_globalManager->set_n(n);
      }

      //Configure segments
      float* segment_1 = surf_globalManager->get_segment_1();
      ImGui::PushItemWidth(100);
      ImGui::DragFloat("Segment 1", segment_1, 0.01, 0.00f, 40.0f, "%.2f");
      float* segment_2 = surf_globalManager->get_segment_2();
      ImGui::PushItemWidth(100);
      ImGui::DragFloat("Segment 2", segment_2, 0.01, 0.00f, 40.0f, "%.2f");

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //Surfacic global piecewise approach
    if(ImGui::TreeNode("Surfacic segmented")){
      Surfacic_segmented* surf_segmentedManager = radioManager->get_Surfacic_segmented();
      ImGui::Separator();
      //---------------------------

      //Plotting
      if(ImGui::Button("plot fitted surface", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        surf_segmentedManager->plot_SurfaceFitting();
      }

      //Polynomial regression degrees
      static int m = 2;
      static int n = 2;
      ImGui::PushItemWidth(100);
      if(ImGui::InputInt("m", &m)){
        surf_segmentedManager->set_m(m);
      }
      ImGui::PushItemWidth(100);
      if(ImGui::InputInt("n", &n)){
        surf_segmentedManager->set_n(n);
      }

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //Surfacic local apprach
    if(ImGui::TreeNode("Surfacic local")){
      Surfacic_local* surf_localManager = radioManager->get_Surfacic_local();
      ImGui::Separator();
      //---------------------------

      //Parameter space
      if(ImGui::Button("Plot space parameter", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          refManager->load_SphereTarget_precomp_add80d();
          surf_localManager->plot_ParameterSpace();
        }
      }

      //Set parameters
      static bool interp_display = false;
      if(ImGui::Checkbox("Display regression", &interp_display)){
        surf_localManager->set_interp_display(interp_display);
      }
      static int Rorder = 4;
      if(ImGui::InputInt("R order", &Rorder)){
        surf_localManager->set_R_order(Rorder);
      }
      static int Itorder = 1;
      if(ImGui::InputInt("It order", &Itorder)){
        surf_localManager->set_It_order(Itorder);
      }
      static int kNN = 20;
      if(ImGui::InputInt("kNN number", &kNN)){
        surf_localManager->set_k_kNN(kNN);
      }
      static int iter = 5;
      if(ImGui::InputInt("Iter.", &iter)){
        surf_localManager->set_iterations(iter);
      }
      static float lambda = 1.0;
      if(ImGui::InputFloat("Lambda", &lambda, 0.01f, 3.0f, "%.3f")){
        surf_localManager->set_lambda(lambda);
      }

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //Surfacic simplified approach
    if (ImGui::TreeNode("Surfacic simplified")){
      Surfacic_simplified* surf_simplManager = radioManager->get_Surfacic_simplified();
      Attribut attribManager(sceneManager);
      ImGui::Separator();
      //---------------------------

      if(ImGui::Button("Compute##1", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          int* algoCorr = radioManager->get_algoCorrection();
          *algoCorr = 1;
          radioManager->correction_oneCloud(cloud);
        }
      }
      if(ImGui::Button("Bundle by classes##1", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          attribManager.compute_attribut_cloud(cloud);
          surf_simplManager->plot_bundleByClass(cloud);
        }
      }
      if(ImGui::Button("Linear regression", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          attribManager.compute_attribut_cloud(cloud);
          surf_simplManager->plot_linearRegression(cloud);
        }
      }
      if(ImGui::Button("Quadratic regression", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          attribManager.compute_attribut_cloud(cloud);
          surf_simplManager->plot_quadraticRegression(cloud);
        }
      }
      if(ImGui::Button("Corrected intensity##1", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          attribManager.compute_attribut_cloud(cloud);
          surf_simplManager->plot_intensityCorrection(cloud);
        }
      }

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //Separation global approach
    if(ImGui::TreeNode("Separation global")){
      Separation_global* sepa_globalManager = radioManager->get_Separation_global();
      ImGui::Separator();
      //---------------------------

      if(ImGui::Button("I(R) regression", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        sepa_globalManager->plot_IbyR();
        sepa_globalManager->plot_IbyR_atomic();
      }
      if(ImGui::Button("I(It) regression", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        sepa_globalManager->plot_IbyIt();
      }
      if(ImGui::Button("e(n) Mean error", ImVec2(150,0))){
        refManager->load_SphereTarget_precomp_add80d();
        sepa_globalManager->plot_MeanError();
      }

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //Simplified radar equation
    if (ImGui::TreeNode("Radar equation")){
      ImGui::Separator();
      //---------------------------

      if(ImGui::Button("Radar equ.", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          radioManager->algo_RadarEquation(0);
        }
      }
      if(ImGui::Button("I.(R²/cos(It))", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          radioManager->algo_RadarEquation(1);
        }
      }
      if(ImGui::Button("I.R²", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          radioManager->algo_RadarEquation(2);
        }
      }
      if(ImGui::Button("I/cos(It)", ImVec2(150,0))){
        if(!sceneManager->get_is_list_empty()){
          radioManager->algo_RadarEquation(3);
        }
      }

      //---------------------------
      ImGui::Separator();
      ImGui::TreePop();
    }

    //---------------------------
    ImGui::Separator();
  }*/
}
