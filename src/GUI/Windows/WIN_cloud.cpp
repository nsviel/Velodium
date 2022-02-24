#include "WIN_cloud.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_maths.h"
#include "../../Operation/Operation_node.h"
#include "../../Operation/Transformation/Attribut.h"

#include "IconsFontAwesome5.h"

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_cloud::WIN_cloud(Operation_node* node_ope){
  //---------------------------

  Engine_node* node_engine = node_ope->get_node_engine();

  this->sceneManager = node_engine->get_SceneManager();
  this->attribManager = node_ope->get_attribManager();

  //---------------------------
}
WIN_cloud::~WIN_cloud(){}

//Main function
void WIN_cloud::window_cloudInfo(){
  Cloud* cloud = sceneManager->get_cloud_selected();
  bool* open = &window_tab.show_modifyFileInfo;
  //---------------------------

  if(*open && cloud != nullptr){
    ImGui::Begin(ICON_FA_COMMENT " Point cloud", open, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoNav);
    //---------------------------

    //Visibility
    static bool visible = true;
    if(ImGui::Checkbox("Visibility", &visible)){
      sceneManager->set_cloudVisibility(cloud, visible);
    }
    ImGui::SameLine();

    //Uniform cloud color
    static vec4 color_PC(0.0f);
    if(cloud != nullptr){
      color_PC = cloud->unicolor;
    }

    ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoInputs;
    flags |= ImGuiColorEditFlags_AlphaBar;
    if(ImGui::ColorEdit4("Color", (float*)&color_PC, flags)){
      if(cloud != nullptr){
        attribManager->set_cloud_color(cloud, color_PC);
      }
    }
    ImGui::Separator();

    //Name
    ImGui::Columns(2);
    ImGui::SetColumnWidth(-1,50);
    static char str[256];
    strcpy(str, cloud->name.c_str());
    ImGui::Text("Name ");
    ImGui::NextColumn();
    if(ImGui::InputText("##1", str, IM_ARRAYSIZE(str), ImGuiInputTextFlags_EnterReturnsTrue)){
      cloud->name = str;
    }
    ImGui::NextColumn();

    //Format
    strcpy(str, cloud->format.c_str());
    ImGui::Text("Format ");
    ImGui::NextColumn();
    static int format = 3;
    if(cloud->format == "pts"){
      format = 0;
    }
    else if(cloud->format == "ptx"){
      format = 1;
    }
    else if(cloud->format == "ply"){
      format = 2;
    }
    else if(cloud->format == "pcap"){
      format = 3;
    }
    if(ImGui::RadioButton("pts", &format, 0)){
      cloud->format = "pts";
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("ptx", &format, 1)){
      cloud->format = "ptx";
    }
    if(ImGui::RadioButton("ply", &format, 2)){
      cloud->format = "ply";
    }
    ImGui::SameLine();
    if(ImGui::RadioButton("pcap", &format, 3)){
      cloud->format = "pcap";
    }
    ImGui::NextColumn();

    //Root pos
    vec3& PCroot = cloud->root;
    ImGui::Text("Root ");
    ImGui::NextColumn();
    ImGui::Text("%.2f, %.2f, %.2f", PCroot.x, PCroot.y, PCroot.z);
    ImGui::SameLine();
    if(ImGui::Button("R", ImVec2(15,0))){
      PCroot = vec3(0,0,0);
    }
    ImGui::NextColumn();

    //Attributs
    ImGui::TextWrapped("Attrib ");
    ImGui::NextColumn();
    ImGui::TextWrapped("%s", cloud->dataFormat.c_str());
    ImGui::Columns(1);
    ImGui::Separator();

    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Functions");
    if(ImGui::Button("Transformation", ImVec2(100,0))){
      window_tab.show_transformation = !window_tab.show_transformation;
    }
    ImGui::SameLine();
    if(ImGui::Button("Data", ImVec2(100,0))){
      window_tab.show_asciiData = !window_tab.show_asciiData;
    }
    ImGui::Separator();

    //Statistics
    ImGui::TextColored(ImVec4(0.4f,0.4f,0.4f,1.0f),"Statistics");
    if(ImGui::Button("Location", ImVec2(100,0))){
      this->cloud_stats_location(cloud);
    }
    ImGui::SameLine();
    if(ImGui::Button("Intensity", ImVec2(100,0))){
      this->cloud_stats_intensity(cloud);
    }
    if(ImGui::Button("Distance", ImVec2(100,0))){
      this->cloud_stats_distance(cloud);
    }
    ImGui::SameLine();
    if(ImGui::Button("cos(It)", ImVec2(100,0))){
      this->cloud_stats_cosIt(cloud);
    }

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }

  //---------------------------
}
void WIN_cloud::window_asciiData(){
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

//Sub functions
void WIN_cloud::cloud_stats_location(Cloud* cloud){
  Subset* subset = sceneManager->get_subset_selected();
  vector<vec3>& XYZ = subset->xyz;
  vec3 XYZ_COM = subset->COM;
  vec3 XYZ_Min = subset->min;
  vec3 XYZ_Max = subset->max;
  //---------------------------

  vector<float> X, Y, Z;
  for(int i=0; i<XYZ.size(); i++){
    X.push_back(XYZ[i].x);
    Y.push_back(XYZ[i].y);
    Z.push_back(XYZ[i].z);
  }

  cout<<"---------"<<endl;
  cout<<"Name : "<<subset->name<<endl;

  if(XYZ.size() != 0){
    cout<<"___XYZ___"<<endl;
    cout<<"COM: "<<XYZ_COM.x<<" "<<XYZ_COM.y<<" "<<XYZ_COM.z<<endl;
    cout<<"Min: "<<XYZ_Min.x<<" "<<XYZ_Min.y<<" "<<XYZ_Min.z<<endl;
    cout<<"Max: "<<XYZ_Max.x<<" "<<XYZ_Max.y<<" "<<XYZ_Max.z<<endl;
    cout<<"Xaxis: min "<<fct_min(X)<<" <-> max "<<fct_max(X)<<" <-> mean "<<fct_mean(X)<<endl;
    cout<<"Yaxis: min "<<fct_min(Y)<<" <-> max "<<fct_max(Y)<<" <-> mean "<<fct_mean(Y)<<endl;
    cout<<"Zaxis: min "<<fct_min(Z)<<" <-> max "<<fct_max(Z)<<" <-> mean "<<fct_mean(Z)<<endl;
    cout<<"__________"<<endl;
  }
}
void WIN_cloud::cloud_stats_intensity(Cloud* cloud){
  Subset* subset = sceneManager->get_subset_selected();
  vector<float>& Is = subset->I;
  //---------------------------

  cout<<"---------"<<endl;
  cout<<"Name : "<<subset->name<<endl;

  if(Is.size() != 0){
    cout<<"___Is___"<<endl;
    cout<<"Min : "<<fct_min(Is)<<endl;
    cout<<"Max : "<<fct_max(Is)<<endl;
    cout<<"Mean : "<<fct_mean(Is)<<endl;
    cout<<"Var : "<<fct_var(Is)<<endl;
    cout<<"Std : "<<fct_std(Is)<<endl;
    cout<<"CV : "<<fct_cv(Is)<<endl;
    cout<<"__________"<<endl;
  }
}
void WIN_cloud::cloud_stats_distance(Cloud* cloud){
  Subset* subset = sceneManager->get_subset_selected();
  vector<float>& dist = subset->R;
  //---------------------------

  cout<<"---------"<<endl;
  cout<<"Name : "<<subset->name<<endl;

  //Distance
  if(dist.size() == 0){
    attribManager->compute_Distances(subset);
  }
  cout<<"___Dist___"<<endl;
  cout<<"Min : "<<fct_min(dist)<<endl;
  cout<<"Max : "<<fct_max(dist)<<endl;
  cout<<"std : "<<fct_std(dist)<<endl;
  cout<<"Mean : "<<fct_mean(dist)<<endl;
  cout<<"__________"<<endl;
}
void WIN_cloud::cloud_stats_cosIt(Cloud* cloud){
  Subset* subset = sceneManager->get_subset_selected();
  vector<float>& cosIt =  subset->cosIt;
  vector<float>& It =  subset->It;
  //---------------------------

  cout<<"---------"<<endl;
  cout<<"Name : "<<subset->name<<endl;

  //Angle d'incidence
  if(cosIt.size() == 0 || It.size() == 0){
    attribManager->compute_cosIt(subset);
  }
  cout<<"___cosIt___"<<endl;
  cout<<"Min : "<<fct_min(cosIt)<<endl;
  cout<<"Max : "<<fct_max(cosIt)<<endl;
  cout<<"Mean : "<<fct_mean(cosIt)<<endl;

  cout<<"___It___"<<endl;
  cout<<"Min : "<<fct_min(It)<<endl;
  cout<<"Max : "<<fct_max(It)<<endl;
  cout<<"std : "<<fct_std(It)<<endl;
  cout<<"Mean : "<<fct_mean(It)<<endl;
  cout<<"__________"<<endl;
}
