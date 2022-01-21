#include "GUI_windows.h"

#include "Window_tab.h"
#include "WIN_loading.h"
#include "WIN_cloudInfo.h"
#include "WIN_camera.h"
#include "WIN_shader.h"
#include "WIN_filter.h"

#include "../../Load/Operation.h"

#include "../../Engine/Engine.h"
#include "../../Engine/Scene.h"
#include "../../Engine/OpenGL/Camera.h"
#include "../../Engine/Glyphs.h"
#include "../../Engine/Configuration/Configuration.h"

#include "../../Operation/Plotting.h"
#include "../../Operation/Functions/Extraction.h"
#include "../../Operation/Functions/Heatmap.h"
#include "../../Operation/Functions/Selection.h"
#include "../../Operation/Optimization/Polyfit.h"
#include "../../Operation/Optimization/Fitting.h"
#include "../../Operation/Transformation/Attribut.h"

#include "../../Specific/fct_display.h"
#include "../../Specific/fct_transtypage.h"
#include "../../Specific/fct_maths.h"

#include "../../../extern/imgui/imgui.h"
#include "../../../extern/IconsFontAwesome5.h"

Window_tab window_tab;


//Constructor / Destructor
GUI_windows::GUI_windows(Engine* engine){
  engineManager = engine;
  //---------------------------

  this->cameraManager = engineManager->get_CameraManager();
  this->heatmapManager = engineManager->get_heatmapManager();

  this->selectionManager = new Selection(engineManager->get_dimManager(), cameraManager);
  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();

  this->opeManager = new Operation();

  this->attribManager = new Attribut();
  this->fitManager = new Fitting();
  this->extractionManager = new Extraction();
  this->plotManager = new Plotting();
  this->configManager = new Configuration();

  this->wincamManager = new WIN_camera(cameraManager);
  this->loadingManager = new WIN_loading();
  this->infoManager = new WIN_cloudInfo();
  this->shaderManager = new WIN_shader(engineManager->get_shaderManager());
  this->filterManager = new WIN_filter(engineManager->get_filterManager());

  //---------------------------
  this->init();
}
GUI_windows::~GUI_windows(){}

//Main functions
void GUI_windows::init(){
  //---------------------------

  window_tab.show_loading = false;
  window_tab.show_saving = false;
  window_tab.show_asciiData = false;
  window_tab.show_camera = false;
  window_tab.show_modifyFileInfo = false;
  window_tab.show_transformation = false;
  window_tab.show_extractCloud = false;
  window_tab.show_cutCloud = false;
  window_tab.show_filter = false;
  window_tab.show_normal = false;
  window_tab.show_intensity = false;
  window_tab.show_color = false;
  window_tab.show_dataOpe = false;
  window_tab.show_selection = false;
  window_tab.show_fitting = false;
  window_tab.show_heatmap = false;
  window_tab.show_shader = false;

  this->cloud_movement = configManager->parse_json_float("transformation", "cloud_movement");

  //---------------------------
}
void GUI_windows::window_Draw(){
  //---------------------------

  this->window_heatmap();
  this->window_asciiData();
  this->window_transformation();
  this->window_selection();
  this->window_extractCloud();
  this->window_cutCloud();
  this->window_normal();
  this->window_intensity();
  this->window_color();
  this->window_dataOpe();
  this->window_fitting();

  wincamManager->window_camera();
  loadingManager->window_loading();
  loadingManager->window_saving();
  infoManager->window_cloudInfo();
  shaderManager->window_shader();
  filterManager->window_filter();

  //---------------------------
}

//General windows
void GUI_windows::window_asciiData(){
  if(window_tab.show_asciiData){
    ImGui::Begin("Data", &window_tab.show_asciiData);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();

    vector<vec3>& XYZ = subset->xyz;
    vector<vec4>& RGB = subset->RGB;
    vector<float>& Is = subset->I;
    vector<vec3>& N = subset->N;
    //---------------------------

    //Settings
    static int nbLines = 100;
    ImGui::SliderInt("Number of lines", &nbLines, 1, subset->nb_point);

    int nb = 2;
    if(nbLines > XYZ.size()) nbLines = XYZ.size();
    if(subset->I.size() != 0) nb++;
    if(subset->has_color) nb++;
    if(subset->N.size() != 0) nb++;

    //Columns
    ImGui::Columns(nb);
    ImGui::Separator();
    ImGui::Text("#");
    ImGui::NextColumn();
    ImGui::Text("XYZ");
    ImGui::NextColumn();
    if(subset->I.size() != 0){
      ImGui::Text("I");
      ImGui::NextColumn();
    }
    if(subset->has_color){
      ImGui::Text("RGB");
      ImGui::NextColumn();
    }
    if(subset->N.size() != 0){
      ImGui::Text("N");
      ImGui::NextColumn();
    }
    ImGui::Separator();

    //Data in columns
    static int selected = -1;
    for(int i=0; i<nbLines; i++){
      ImGui::TextColored(ImVec4(0.4f,0.9f,0.4f,1.0f),"%i", i+1);
      ImGui::NextColumn();

      ImGui::Text("%f %f %f", XYZ[i].x, XYZ[i].y, XYZ[i].z);
      ImGui::NextColumn();

      if(subset->I.size() != 0){
        ImGui::Text("%f", Is[i]);
        ImGui::NextColumn();
      }

      if(subset->has_color){
        ImGui::Text("%f %f %f", RGB[i].x, RGB[i].y, RGB[i].z);
        ImGui::NextColumn();
      }

      if(subset->N.size() != 0){
        ImGui::Text("%f %f %f", N[i].x, N[i].y, N[i].z);
        ImGui::NextColumn();
      }
    }
    ImGui::Columns(1);
    ImGui::TreePop();

    //---------------------------
    ImGui::End();
  }
}
void GUI_windows::window_heatmap(){
  if(window_tab.show_heatmap){
    ImGui::Begin(ICON_FA_EYE " Heatmap", &window_tab.show_heatmap, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Apply heatMap on one cloud
    if(ImGui::Button("Apply", ImVec2(75,0))){
      if(cloud != nullptr){
        heatmapManager->set_Heatmap(cloud);
      }
    }
    ImGui::SameLine();

    //Heatmap all clouds
    static bool heatAll = false;
    if(ImGui::Button("Apply all", ImVec2(75,0))){
      if(cloud != nullptr){
        heatAll = !heatAll;
        heatmapManager->set_Heatmap_all(heatAll);
      }
    }

    //Select heatmap channel
    static int style_idx = 0;

    int* HMmode = heatmapManager->get_HeatmapField();
    ImGui::SetNextItemWidth(75);
    ImGui::Combo("##1", HMmode, "height\0Is\0dist\0cos(It)\0It\0");
    ImGui::SameLine();

    //Normalize palette
    bool* normalizeON = heatmapManager->get_param_Normalized();
    ImGui::Checkbox("Normalized", normalizeON);

    //Height range configuration
    if(*HMmode == 0){
      vec2* range = heatmapManager->get_height_range();
      if(ImGui::DragFloatRange2("Height range", &range->x, &range->y, 0.01f, -20.0f, 50.0f, "%.2f", "%.2f")){
        heatmapManager->compute_subset_heatmap_ON(subset);
        sceneManager->update_subset_color(subset);
      }
    }

    //Intensity range configuration
    if(*HMmode == 1){
      //Set heatmap range
      vec2 heatmap_range = *heatmapManager->get_heatmap_range();
      int min = (int) (heatmap_range.x * 255);
      int max = (int) (heatmap_range.y * 255);

      if(ImGui::DragIntRange2("Intensity", &min, &max, 1, 0, 255, "%d", "%d")){
        vec2* range = heatmapManager->get_heatmap_range();
        *range = vec2((float)min / 255, (float)max / 255);
      }
    }

    //Display color palette
    if(ImGui::Button("Palette", ImVec2(75,0))){
      if(cloud != nullptr && cloud->heatmap){
        heatmapManager->plot_colorPalette(subset);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_heatmap = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_transformation(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  Subset* subset = sceneManager->get_subset_selected();

  if(window_tab.show_transformation && cloud != nullptr){
    ImGui::Begin("Transformation", &window_tab.show_transformation, ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    //Z scanner
    /*ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Cloud elevation");

    //One or all cloud to operate
    static bool allClouds = false;
    ImGui::Checkbox("All clouds", &allClouds);

    static float Z_approx = 0.0f;
    static float Zpos = 0.0f;
    ImGui::PushItemWidth(75);
    if(ImGui::DragFloat("Z", &Zpos, 0.01f)){
      if(cloud != nullptr){
        transformManager.make_elevation(cloud, Zpos);
        sceneManager->update_cloud_location(cloud);
        Z_approx = transformManager.fct_soilDetermination(cloud);
      }
    }
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), "Z : %.3f", Z_approx);

    //Scanner height from ground
    static float Z_scan = 0.0f;
    if(ImGui::DragFloat("Scanner height", &Z_scan, 0.05f)){
      if(cloud != nullptr){
        Z_approx = transformManager.fct_soilDetermination(cloud);
      }
    }
    ImGui::SameLine();
    static int soilnb_point = 10000;
    if(ImGui::DragInt("Ground pts", &soilnb_point, 100)){
      if(cloud != nullptr){
        transformManager.set_soilnb_point(soilnb_point);
        Z_approx = transformManager.fct_soilDetermination(cloud);
      }
    }

    if(ImGui::Button("Accept##0")){
      if(allClouds){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0;i<list_cloud->size();i++){
          Cloud* cloud = *next(list_cloud->begin(),i);
          transformManager.fct_adjustPosToScanner(cloud, Z_scan);
          sceneManager->update_cloud_location(cloud);
        }
      }
      else{
        if(cloud != nullptr){
          transformManager.fct_adjustPosToScanner(cloud, Z_scan);
          sceneManager->update_cloud_location(cloud);
          Z_approx = transformManager.fct_soilDetermination(cloud);
        }
      }
    }
    ImGui::Separator();*/
    //---------------------------

    //Transformation matrix from initial
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Actual transformation from initial position");
    MatrixXf cloudTransformation = glm_to_eigen_mat4_matXf(subset->transformation);
    std::stringstream ss;
    ss << cloudTransformation;
    string bla = ss.str();
    static char cloudTransformation_c[1024 * 16];
    strcpy(cloudTransformation_c, bla.c_str());
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine;
    ImGui::InputTextMultiline("##source", cloudTransformation_c, IM_ARRAYSIZE(cloudTransformation_c), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

    //Real transformation matrix registration
    if(ImGui::CollapsingHeader("Real transformation matrix")){
      MatrixXf realTransformation = glm_to_eigen_mat4_matXf(subset->transformation);//transformation.RealTransformation);
      std::stringstream ss1;
      ss1 << realTransformation;
      string bla1 = ss1.str();
      static char realTransformation_c[1024 * 16];
      strcpy(realTransformation_c, bla1.c_str());
      ImGui::InputTextMultiline("##realTransfo", realTransformation_c, IM_ARRAYSIZE(realTransformation_c), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

      if(ImGui::Button("Apply real transformation from init", ImVec2(300,0))){
        sceneManager->update_cloud_reset(cloud);
        transformManager.make_Transformation(subset, vec3(0,0,0), subset->transformation);//transformation.RealTransformation);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::Separator();
    //---------------------------

    //Applicable transformation matrix
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

    ImGui::InputTextMultiline("##source2", TransfoMatrix, IM_ARRAYSIZE(TransfoMatrix), ImVec2(400, ImGui::GetTextLineHeight() * 6), flags);

    if(ImGui::Button("Apply from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        Cloud* cloud = sceneManager->get_cloud_selected();
        Subset* subset = sceneManager->get_subset_selected();

        sceneManager->update_cloud_reset(cloud);
        transformManager.make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Apply from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);

        //------------------
        transformManager.make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }
    if(ImGui::Button("Reverse from initial pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        Cloud* cloud = sceneManager->get_cloud_selected();
        Subset* subset = sceneManager->get_subset_selected();

        sceneManager->update_cloud_reset(cloud);
        transformManager.make_Transformation(subset, vec3(0,0,0), mat);
        sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Reverse from actual pos", ImVec2(150,0))){
      if(cloud != nullptr){
        mat4 mat = char_to_glm_mat4(TransfoMatrix);
        mat4 mat2 = inverse(mat);

        transformManager.make_Transformation(subset, vec3(0,0,0), mat);
        //sceneManager->update_cloud_location(cloud);
      }
    }
    ImGui::Separator();

    //---------------------------
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Cloud translation");
    ImGui::PushItemWidth(150);
    static float trans[3] = {0.0f, 0.0f, 0.0f};
    ImGui::DragFloat3("XYZ", trans, 0.01f, -10.0f, 10.0f);
    ImGui::SameLine();
    if(ImGui::Button("Apply##1")){
      if(cloud != nullptr){
        vec3 translation = vec3(trans[0], trans[1], trans[2]);
        transformManager.make_translation(cloud, translation);
        sceneManager->update_cloud_location(cloud);
        trans[0] = 0;
        trans[1] = 0;
        trans[2] = 0;
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_transformation = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_fitting(){
  if(window_tab.show_fitting){
    ImGui::Begin("Fitting", &window_tab.show_fitting,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    int sizeButton = 150;
    //---------------------------

    //Sphere fitting
    if(ImGui::Button("Sphere fitting", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        fitManager->Sphere_cloudToCenter_all();
      }
    }

    //Plane fitting
    if(ImGui::Button("Plane fitting", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        //fitManager->Plane_cloud(subset);
      }
    }

    //Axis alignement
    if(ImGui::Button("X alignement", ImVec2(sizeButton,0))){
      if(cloud != nullptr){
        transformManager.make_orientAxis_X(cloud);
        transformManager.make_alignAxis_X(cloud);
        sceneManager->update_cloud_location(cloud);
      }
    }

    //---------------------------
    if(ImGui::Button("Close")){
      window_tab.show_fitting = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_normal(){
  if(window_tab.show_normal){
    ImGui::Begin("Attributs", &window_tab.show_normal,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    if(ImGui::Button("Compute attributs for all clouds", ImVec2(200,0))){
      attribManager->compute_cloudAttributs_all();
    }

    //Standard normal computation
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Normals");

    static int normalMethod = 0;
    ImGui::PushItemWidth(207.5);
    ImGui::Combo("##11", &normalMethod, "PCL\0Hough\0Sphere fitting\0Plane fitting\0X axis\0Y axis\0Z axis\0");

    static float radius_normal = 0.03f;
    if(normalMethod == 1){
      ImGui::SetNextItemWidth(100);
      if(ImGui::InputFloat("Hough radius", &radius_normal, 0.01f, 1.0f, "%.3f")){
        if(radius_normal < 0.01) radius_normal = 0.01;
        //attribManager->set_normalRadiusSeach(radius_normal);
      }
    }
    if(normalMethod == 2){
      static float radius = attribManager->get_sphereRadius();
      ImGui::SetNextItemWidth(100);
      if(ImGui::DragFloat("Sphere radius", &radius, 0.0001, 0, 1, "%.4f")){
        attribManager->set_sphereRadius(radius);
      }
    }

    if(ImGui::Button("Compute", ImVec2(200,0))){
      if(cloud != nullptr){
        //---------------------------
        Subset* subset = &cloud->subset[cloud->subset_selected];

        if(normalMethod == 0){
          attribManager->compute_normals(subset);
        }

        if(normalMethod == 1){
          attribManager->compute_normals_Hough(subset);
        }

        if(normalMethod == 2){
          attribManager->compute_normals_sphere(subset);
        }

        if(normalMethod == 3){
          attribManager->compute_normals_planFitting(subset);
        }

        if(normalMethod == 4){
          float angle = transformManager.make_orientAxis_X(cloud);
          attribManager->compute_normals_planXaxis(subset);
          vec3 rotation = vec3(0, 0, -angle);
          transformManager.make_rotation(cloud, vec3(0,0,0), rotation);
          cloud->subset_init[0].N = subset->N;
          sceneManager->update_cloud_location(cloud);
        }

        if(normalMethod == 5){
          attribManager->compute_normals_planYaxis(subset);
          cloud->subset_init[0].N = subset->N;
          sceneManager->update_subset_location(subset);
        }

        if(normalMethod == 6){
          attribManager->compute_normals_planZaxis(subset);
          cloud->subset_init[0].N = subset->N;
          sceneManager->update_cloud_location(cloud);
        }

        glyphManager->update_glyph_object("normal", cloud);

        //---------------------------
      }
    }
    if(ImGui::Button("Compute all clouds", ImVec2(200,0))){
      if(cloud != nullptr){
        //---------------------------


        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0;i<list_cloud->size();i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          if(normalMethod == 0){
            attribManager->compute_normals(subset);
          }

          if(normalMethod == 1){
            attribManager->compute_normals_Hough(subset);
          }

          if(normalMethod == 2){
            attribManager->compute_normals_sphere(subset);
          }

          if(normalMethod == 3){
            attribManager->compute_normals_planFitting(subset);
          }

          if(normalMethod == 4){
            float angle = transformManager.make_orientAxis_X(cloud);
            attribManager->compute_normals_planXaxis(subset);
            vec3 rotation = vec3(0, 0, -angle);
            transformManager.make_rotation(cloud, vec3(0,0,0), rotation);
            cloud->subset_init[0].N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          if(normalMethod == 5){
            attribManager->compute_normals_planYaxis(subset);
            cloud->subset_init[0].N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          if(normalMethod == 6){
            attribManager->compute_normals_planZaxis(subset);
            cloud->subset_init[0].N = subset->N;
            sceneManager->update_subset_location(subset);
          }

          glyphManager->update_glyph_object("normal", cloud);
        }

        //---------------------------
      }
    }

    if(ImGui::Button("Reoriente to origin", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_reorientToOrigin(subset);
        glyphManager->update_glyph_object("normal", cloud);
      }
    }
    if(ImGui::Button("Invert", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_normals_invert();
        glyphManager->update_glyph_object("normal", cloud);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_normal = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_intensity(){
  if(window_tab.show_intensity){
    ImGui::Begin("Intensity", &window_tab.show_intensity, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity functions");

    //Display intensity or color channel
    static bool colorON = false;
    if(ImGui::Button("Intensity / Color all", ImVec2(200,0))){
      colorON = !colorON;
      if(colorON){
        attribManager->set_colorRGB_all();
      }else{
        attribManager->set_colorI_all();
      }
    }

    //Invert the intensity values
    if(ImGui::Button("Inversion Is", ImVec2(200,0))){
      if(cloud != nullptr){
        attribManager->compute_intensityInversion();
        sceneManager->update_subset_IntensityToColor(subset);
      }
    }

    //Normalize the intensity values
    if(ImGui::Button("Normalize Intensity to [0,1]", ImVec2(200,0))){
      vector<float>& Is = subset->I;
      Is = fct_normalize(Is);
      sceneManager->update_subset_IntensityToColor(subset);
    }

    //Intensity display slider
    ImGui::Text("Selection intensity");
    static float min = 0, max = 1;
    if(ImGui::DragFloatRange2("##123321", &min, &max, 0.001f, 0.00f, 1.0f, "%.3f", "%.3f")){
      subset->I = cloud->subset_init[0].I;
      attribManager->fct_IsRange(vec2(min, max));
    }

    //Intensity shifting
    ImGui::Text("Shift intensity");
    static float shift = 0.01;
    ImGui::PushItemWidth(100);
    ImGui::InputFloat("##123322", &shift, 0.0f, 1.0f, "%.4f");
    ImGui::SameLine();
    ImGuiStyle& style = ImGui::GetStyle();
    float spacing = style.ItemInnerSpacing.x;
    ImGui::PushButtonRepeat(true);
    if(ImGui::ArrowButton("##left", ImGuiDir_Left)){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i] - shift;
        if(Is[i] < 0.0f) Is[i] = 0.0f;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine(0.0f, spacing);
    if(ImGui::ArrowButton("##right", ImGuiDir_Right)){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i] + shift;
        if(Is[i] > 1.0f) Is[i] = 1.0f;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::PopButtonRepeat();
    ImGui::Separator();

    //Reconvert intensity
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Intensity scaling");
    if(ImGui::Button("Restore I initial", ImVec2(200,0))){
      subset->I = cloud->subset_init[0].I;
      sceneManager->update_subset_IntensityToColor(subset);
    }
    if(ImGui::Button("I:255->2048", ImVec2(100,0))){
      attribManager->fct_convert255to2048(subset);
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine();
    if(ImGui::Button("I:2048->255", ImVec2(100,0))){
      attribManager->fct_convert2048to255(subset);
      sceneManager->update_subset_IntensityToColor(subset);
    }
    if(ImGui::Button("I:1->2048", ImVec2(100,0))){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = Is[i]*4096-2048;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }
    ImGui::SameLine();
    if(ImGui::Button("I:2048->1", ImVec2(100,0))){
      vector<float>& Is = subset->I;
      for(int i=0; i<Is.size(); i++){
        Is[i] = (Is[i]+2048)/4096;
      }
      sceneManager->update_subset_IntensityToColor(subset);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_intensity = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_color(){
  if(window_tab.show_color){
    ImGui::Begin("Color", &window_tab.show_color, ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Color channel
    ImGui::Text("Color channel");
    static int e = 0;
    ImGui::Separator();
    ImGui::Columns(2);
    ImGui::Text("Selected cloud"); ImGui::NextColumn();
    ImGui::Text("All cloud"); ImGui::NextColumn();
    if(ImGui::RadioButton("I    ##1", &e, 1)){
      if(subset->I.size() != 0){
        sceneManager->update_subset_IntensityToColor(subset);
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("I    ##2", &e, 2)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->I.size() != 0){
          sceneManager->update_subset_IntensityToColor(subset);
        }
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB  ##1", &e, 3)){
      if(subset->has_color){
        subset->RGB = subset->RGB;
        sceneManager->update_subset_color(subset);
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB  ##2", &e, 4)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->has_color){
          subset->RGB = subset->RGB;
          sceneManager->update_subset_color(subset);
        }
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB*I##1", &e, 5)){
      if(subset->I.size() != 0 && subset->has_color){
        attribManager->set_enhancedColor(subset);
      }else{
        cout<<"Selected cloud: I="<<subset->I.size()<<" | Color="<<subset->has_color<<endl;
      }
    } ImGui::NextColumn();
    if(ImGui::RadioButton("RGB*I##2", &e, 6)){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->I.size() != 0 && subset->has_color){
          attribManager->set_enhancedColor(subset);
        }
      }
    } ImGui::NextColumn();
    ImGui::Columns(1);
    ImGui::Separator();

    //Color functions
    if(ImGui::Button("Supress color all clouds", ImVec2(150,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->has_color){
          subset->RGB.clear();
        }
        subset->has_color = false;
        sceneManager->update_subset_dataFormat(subset);
      }
    }
    if(ImGui::Button("Random color for all points", ImVec2(150,0))){
      attribManager->set_randomizeAllPointsColor(subset);
      sceneManager->update_subset_color(subset);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_color = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_selection(){
  if(window_tab.show_selection){
    ImGui::Begin("Selection part", &window_tab.show_selection,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    ImGui::Text("Point");
    static bool selectionPtON = false;
    if(ImGui::Checkbox("Selection mode", &selectionPtON)){
      if(cloud != nullptr && selectionPtON){
        heatmapManager->set_Heatmap_all(true);
        selectionManager->set_markMode("sphere");
        cloud->point_size = 10;
        cloud_movement = false;
      }

      if(!selectionPtON){
        heatmapManager->set_Heatmap_all(false);
        selectionManager->set_markMode("cube");
        cloud->point_size = 1;
        cloud_movement = true;
      }
    }
    static float sensibility = 0.005f;
    if(ImGui::DragFloat("Sensibility", &sensibility, 0.0001, 0, 1, "%.4f")){
      selectionManager->set_selectionSensibility(sensibility);
    }
    if(ImGui::Button("Supress all points", ImVec2(200,0))){
      selectionManager->mark_supressSelectedPoints_all();
      //heatmapManager->set_Heatmap_all(true);
    }
    ImGui::Separator();

    ImGui::Text("Cloud part");
    static float xmin = 0, xmax = 100;
    static float ymin = 0, ymax = 100;
    static float zmin = 0, zmax = 100;
    if(ImGui::Button("Reset", ImVec2(100,0)) || ImGui::IsKeyPressed(258)){
      xmin = 0; xmax = 100;
      ymin = 0; ymax = 100;
      zmin = 0; zmax = 100;
    }
    ImGui::SameLine();
    bool* highlightON = extractionManager->get_highlightON();
    if(ImGui::Checkbox("Hightligth", highlightON) || ImGui::IsKeyPressed(258)){
      if(cloud != nullptr){
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);
      }
    }

    //AABB manipulators
    ImGui::PushAllowKeyboardFocus(false);
    if(ImGui::DragFloatRange2("X", &xmin, &xmax, 0.25f, 0.01f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    ImGui::PopAllowKeyboardFocus();

    if(ImGui::Button("Extract part", ImVec2(100,0))){
      if(cloud != nullptr){
        vec3 min_pourc = vec3(xmin, ymin, zmin);
        vec3 max_pourc = vec3(xmax, ymax, zmax);
        extractionManager->fct_selectPart(subset, min_pourc, max_pourc);
      }
    }

    //Table of selected parts
    list<subpart*>* list = extractionManager->get_listParts();
    ImGui::Columns(5, "part");
    ImGui::Separator();
    ImGui::Text("ID"); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::NextColumn();
    ImGui::Text("Cloud"); ImGui::NextColumn();
    ImGui::Text("Min"); ImGui::NextColumn();
    ImGui::Text("Max"); ImGui::NextColumn();
    ImGui::Separator();
    static int selected = -1;

    ImGui::SetColumnWidth(0,50);
    ImGui::SetColumnWidth(1,125);

    for (int i=0; i<list->size(); i++){
      subpart* part = *next(list->begin(),i);

      //ID
      char label[32];
      sprintf(label, "%04d", i);
      if(ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_None)){
        selected = i;
      }
      bool hovered = ImGui::IsItemHovered();
      ImGui::NextColumn();

      //Name
      static char str[256];
      strcpy(str, part->name.c_str());
      string truc = "##" + to_string(i);
      const char* ID_label = truc.c_str();
      if(ImGui::InputText(ID_label, str, IM_ARRAYSIZE(str))){
        part->name = str;
      }

      //Removal cross
      ImGui::PushID(i);
      ImGui::SameLine();
      if(ImGui::SmallButton("X")){
        extractionManager->supress_selectedpart(part);
      }
      ImGui::PopID();
      ImGui::NextColumn();

      //Cloud
      ImGui::Text("%s", part->namePC.c_str());
      ImGui::NextColumn();

      //Min
      vec3 minloc = part->minloc;
      ImGui::Text("%.1f %.1f %.1f", minloc.x, minloc.y, minloc.z);
      ImGui::NextColumn();

      //Max
      vec3 maxloc = part->maxloc;
      ImGui::Text("%.1f %.1f %.1f", maxloc.x, maxloc.y, maxloc.z);
      ImGui::NextColumn();
    }
    ImGui::Columns(1);
    ImGui::Separator();

    //---------------------------
    if(ImGui::Button("Close")){
      window_tab.show_selection = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_extractCloud(){
  if(window_tab.show_extractCloud){
    ImGui::Begin("Extract cloud", &window_tab.show_extractCloud,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    //Extraction functions
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from AABB manipulators");
    bool* highlightON = extractionManager->get_highlightON();
    if(ImGui::Checkbox("Hightligth", highlightON)){
      if(cloud != nullptr){
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);
      }
    }

    //Reset manipulators
    static float xmin = 0, xmax = 100;
    static float ymin = 0, ymax = 100;
    static float zmin = 0, zmax = 100;
    if(ImGui::Button("Reset X Y Z", ImVec2(100,0))){
      xmin = 0;
      xmax = 100;
      ymin = 0;
      ymax = 100;
      zmin = 0;
      zmax = 100;
    }

    //AABB manipulators
    ImGui::PushAllowKeyboardFocus(false);
    if(ImGui::DragFloatRange2("X", &xmin, &xmax, 0.25f, 0.01f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    ImGui::PopAllowKeyboardFocus();

    //Extract a new cloud from AABB manipulators
    if(ImGui::Button("Extract cloud", ImVec2(100,0))){
      if(cloud != nullptr){
        //Reset color
        *highlightON = false;
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);

        //Extract cloud
        extractionManager->fct_extractCloud(cloud);
      }
    }
    ImGui::SameLine();
    static bool sliceON = false;
    if(ImGui::Checkbox("Slice", &sliceON)){
      extractionManager->set_sliceON(sliceON);
    }
    ImGui::Separator();

    //Extract points selected with the mouse frame
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Extract from mouse frame");
    if(ImGui::Button("Extract selected frame", ImVec2(150,0))){
      if(cloud != nullptr){
        extractionManager->fct_extractSelected(cloud);
      }
    }
    ImGui::Separator();

    //Merge and extract two clouds
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Merge and extract two clouds");
    if(ImGui::Button("Merge clouds", ImVec2(150,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      if(list_cloud->size() >= 2){
        Cloud* cloud_2 = sceneManager->get_othercloud();
        extractionManager->fct_merging_newCloud(cloud, cloud_2);
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_extractCloud = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_cutCloud(){
  if(window_tab.show_cutCloud){
    ImGui::Begin("Cut cloud", &window_tab.show_cutCloud,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    bool* highlightON = extractionManager->get_highlightON();
    if(ImGui::Checkbox("Hightligth", highlightON) || ImGui::IsKeyPressed(258)){
      if(cloud != nullptr){
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);
      }
    }
    ImGui::SameLine();

    //Reset manipulator
    static float xmin = 0, xmax = 100;
    static float ymin = 0, ymax = 100;
    static float zmin = 0, zmax = 100;
    if(ImGui::Button("Reset X Y Z", ImVec2(100,0))){
      xmin = 0;
      xmax = 100;
      ymin = 0;
      ymax = 100;
      zmin = 0;
      zmax = 100;
    }

    //AABB manipulators
    ImGui::PushAllowKeyboardFocus(false);
    if(ImGui::DragFloatRange2("X", &xmin, &xmax, 0.25f, 0.01f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Y", &ymin, &ymax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    if(ImGui::DragFloatRange2("Z", &zmin, &zmax, 0.25f, 0.0f, 100.0f, "%.1f %%", "%.1f %%")){
      if(cloud != nullptr){
        extractionManager->set_AABB_min(vec3(xmin,ymin,zmin));
        extractionManager->set_AABB_max(vec3(xmax,ymax,zmax));
        glyphManager->update_glyph_object("aabb", cloud);
      }
    }
    ImGui::PopAllowKeyboardFocus();

    //Cuttinf functions
    if(ImGui::Button("Cut", ImVec2(100,0))){
      if(cloud != nullptr){
        //Reset color
        *highlightON = false;
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);

        //Cut cloud
        extractionManager->fct_cutCloud(subset);
      }
    }
    ImGui::SameLine();
    if(ImGui::Button("Cut all cloud", ImVec2(100,0))){
      if(cloud != nullptr){
        //Reset color
        *highlightON = false;
        Subset* subset = &cloud->subset[cloud->subset_selected];
        Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
        extractionManager->fct_highlighting(subset, subset_init);

        //Cut clouds
        extractionManager->fct_cutCloud_all();
      }
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_cutCloud = false;
    }
    ImGui::End();
  }
}
void GUI_windows::window_dataOpe(){
  if(window_tab.show_dataOpe){
    ImGui::Begin("Data", &window_tab.show_dataOpe,ImGuiWindowFlags_AlwaysAutoResize);
    Cloud* cloud = sceneManager->get_cloud_selected();
    Subset* subset = sceneManager->get_subset_selected();
    //---------------------------

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Write on file");
    if(ImGui::Button("I-R-a all without ref", ImVec2(200,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

      //Write data on file
      ofstream file;
      file.open ("../../data/data_IRA_woRef.txt");
      file << "Name "<<"R "<<"alpha "<<"IRAWmean "<<"IRAWstd "<<"ICORmean "<<"ICORstd "<<"\n";
      file << "--------------------------------"<<"\n";

      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->name.find("Sphere") == std::string::npos &&
        subset->name.find("Spectralon") == std::string::npos){
          if(subset->R.size() == 0) attribManager->compute_Distances(subset);
          if(subset->It.size() == 0) attribManager->compute_cosIt(subset);

          float R = fct_mean(subset->R);
          float A = fct_mean(subset->It);

          float Irm = fct_mean(subset->I);
          float Irs = fct_std(subset->I);
          float Icm = fct_mean(subset->I);
          float Ics = fct_std(subset->I);

          file <<std::fixed;
          file <<subset->name<<" ";
          file <<setprecision(3)<< R<<" "<<A<<" ";
          file <<setprecision(3)<< Irm<<" "<<Irs<<" ";
          file <<setprecision(3)<< Icm<<" "<<Ics<<"\n";
          //---------------------------
        }
      }
      file.close();
    }
    if(ImGui::Button("I all without ref", ImVec2(200,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

      //Write data on file
      ofstream file;
      file.open ("../../data/data_IRA_woRef.txt");
      file << "Imean "<<"Istd "<<"\n";
      file << "--------------------------------"<<"\n";

      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->name.find("Sphere") == std::string::npos &&
        subset->name.find("Spectralon") == std::string::npos){

          float Im = fct_mean(subset->I);
          float Is = fct_std(subset->I);

          file <<std::fixed;
          file <<setprecision(3)<< Im<<" "<<Is<<"\n";
          //---------------------------
        }
      }
      file.close();
    }
    if(ImGui::Button("R-Alpha all without ref", ImVec2(200,0))){
      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

      //Write data on file
      ofstream file;
      file.open ("../../data/data_IRA_woRef.txt");
      file << "R "<<"Alpha "<<"\n";
      file << "--------------------------------"<<"\n";

      for(int i=0;i<list_cloud->size();i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        if(subset->name.find("Sphere") == std::string::npos &&
        subset->name.find("Spectralon") == std::string::npos){
          if(subset->R.size() == 0) attribManager->compute_Distances(subset);
          if(subset->It.size() == 0) attribManager->compute_cosIt(subset);

          float R = fct_mean(subset->R);
          float A = fct_mean(subset->It);

          file <<std::fixed;
          file <<setprecision(3)<< R<<" "<<A<<"\n";
          //---------------------------
        }
      }
      file.close();
    }
    if(ImGui::Button("Write data in file", ImVec2(200,0))){
      ofstream file;
      file.open ("../../data/data_allCloud.txt");
      file << "Name R alpha I"<<"\n";
      file << "--------------------------------"<<"\n";
      //---------------------------

      list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
      for(int i=0; i<list_cloud->size(); i++){
        Cloud* cloud = *next(list_cloud->begin(),i);

        float R = fct_mean(subset->R);
        float A = fct_mean(subset->It);
        float I = fct_mean(subset->I);

        //file <<setprecision(5)<<subset->name<<" "<<R<<" "<<A<<" "<<I<<"\n";
        file <<setprecision(5)<<R<<" "<<A<<" "<<I<<"\n";
      }

      //---------------------------
      file.close();
    }
    if(ImGui::Button("Write intensity in file", ImVec2(200,0))){
      ofstream file;
      file.open ("../../data/data_intensity.txt");

      Cloud* cloud = sceneManager->get_cloud_selected();
      Subset* subset = sceneManager->get_subset_selected();

      vector<float>& Is = subset->I;
      //---------------------------

      for(int i=0; i<Is.size(); i++){
        file <<setprecision(4)<<Is[i]<<"\n";
      }

      //---------------------------
      file.close();
    }

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Write on term");
    if(ImGui::Button("I Istd all", ImVec2(200,0))){
      //---------------------

      if(cloud != nullptr){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

        //Intensity
        cout<<"-- Intensity --"<<endl;
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          cout<<std::fixed<<std::setprecision(2)<<"& "<<fct_mean(subset->I)<<endl;
        }

        //Standard deviation
        cout<<"-- SD --"<<endl;
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          cout<<std::fixed<<std::setprecision(3)<<"& "<<fct_std(subset->I)<<endl;
        }
      }

      //---------------------
    }
    if(ImGui::Button("Min-Max R It all", ImVec2(200,0))){
      vector<float> min_R, min_It;
      vector<float> max_R, max_It;
      //---------------------

      if(cloud != nullptr){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          vector<float>& dist = subset->R;
          if(dist.size() == 0) attribManager->compute_Distances(subset);
          min_R.push_back(fct_min(dist));
          max_R.push_back(fct_max(dist));

          vector<float>& It = subset->It;
          if(It.size() == 0) attribManager->compute_cosIt(subset);
          min_It.push_back(fct_min(It));;
          max_It.push_back(fct_max(It));;
        }
      }

      //---------------------
      cout<<"R: Min = "<<fct_min(min_R)<<" | Max = "<<fct_max(max_R)<<endl;
      cout<<"It: Min = "<<fct_min(min_It)<<" | Max = "<<fct_max(max_It)<<endl;
    }
    if(ImGui::Button("R It all", ImVec2(200,0))){
      //---------------------

      if(cloud != nullptr){
        list<Cloud*>* list_cloud = sceneManager->get_list_cloud();

        //Distances
        cout<<"-- Distances --"<<endl;
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          vector<float>& dist = subset->R;
          if(dist.size() == 0) attribManager->compute_Distances(subset);

          cout<<std::fixed<<std::setprecision(2)<<"& "<<fct_mean(dist)<<endl;
        }

        //Angle
        cout<<"-- Angles --"<<endl;
        for(int i=0; i<list_cloud->size(); i++){
          Cloud* cloud = *next(list_cloud->begin(),i);

          vector<float>& It = subset->It;
          if(It.size() == 0) attribManager->compute_cosIt(subset);

          cout<<std::fixed<<std::setprecision(2)<<"& "<<fct_mean(It)<<endl;
        }

      }

      //---------------------
    }

    if(ImGui::Button("Test regression", ImVec2(200,0))){
      vector<float> X{
        1.30806,
        2.29148,
        3.28815,
        4.2841,
        5.29482,
        7.32926,
        9.30038,
        11.3138,
        13.3208,
        15.3293,
        17.3446,
        19.3401,
        21.3557,
        23.3466,
        25.3535,
        27.3686,
        29.3698};
      vector<float> Y{
        0.918878,
        0.89659,
        0.869665,
        0.895808,
        0.94005,
        0.938376,
        0.897884,
        0.858437,
        0.825976,
        0.820597,
        0.823466,
        0.827613,
        0.829898,
        0.834169,
        0.836656,
        0.840727,
        0.840009};
      vector<float> X_low{
        5.29482,
        7.32926,
        9.30038,
        13.3208,
        15.3293,
        17.3446,
        21.3557,
        23.3466,
        29.3698};
      vector<float> Y_low{
        0.94005,
        0.938376,
        0.897884,
        0.825976,
        0.820597,
        0.823466,
        0.829898,
        0.834169,
        0.840009};
      vector<float> X_short{
        5.29482,
        6,
        7.32926,
        9.30038};
      vector<float> Y_short{
        0.94005,
        0.94,
        0.938376,
        0.897884};
      int n = 8;

      X = X_low;
      Y = Y_low;

      vector<float> coeff = polyfit(X, Y, n);
      vector<float> reg = polyval(X, coeff, n);

      plotManager->set_Xlabel("X");
      plotManager->set_Ylabel("Y");
      plotManager->set_Format_data1("with linespoints ls 1 pt 13 ps 0.5 lc rgb 'black' title 'Initial'");
      plotManager->set_Format_data2("with linespoints ls 1 pt 13 ps 0.5 lc rgb 'red' title 'Regression'");
      plotManager->plot_Regression(X, Y, reg);
      //-------------------

      coeff = polyfit_homemade(X, Y, n);
      reg = polyval(X, coeff, n);

      plotManager->set_Xlabel("X");
      plotManager->set_Ylabel("Y");
      plotManager->set_Format_data1("with linespoints ls 1 pt 13 ps 0.5 lc rgb 'black' title 'Initial'");
      plotManager->set_Format_data2("with linespoints ls 1 pt 13 ps 0.5 lc rgb 'red' title 'Regression'");
      plotManager->plot_Regression(X, Y, reg);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      window_tab.show_dataOpe = false;
    }
    ImGui::End();
  }
}
