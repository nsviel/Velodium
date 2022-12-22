#include "Radiometry.h"

#include "Plot_radio.h"
#include "Target/Reference.h"
#include "Target/Ref_Operation.h"

#include "Approach/Linearization.h"
#include "Approach/RadarEquation.h"
#include "Approach/Surfacic_simplified.h"
#include "Approach/Surfacic_global_piecewise.h"
#include "Approach/Surfacic_local.h"
#include "Approach/Surfacic_segmented.h"
#include "Approach/Separation_global.h"
#include "Approach/Separation_local.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Node_operation.h"
#include "../../../Operation/Function/Plotting.h"
#include "../../../Operation/Transformation/Attribut.h"
#include "../../../Specific/fct_math.h"


//Constructor / destructor
Radiometry::Radiometry(Node_engine* node){
  //---------------------------

  Node_operation* node_ope = node->get_node_ope();

  this->sceneManager = node->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();
  this->refManager = new Reference(node);
  this->refopeManager = new Ref_Operation(refManager);
  this->plotManager = new Plotting();
  this->linManager = new Linearization(sceneManager, refopeManager);
  this->radioplotManager = new Plot_radio(node);

  this->radio_radar = new RadarEquation();
  this->radio_surf_simpl = new Surfacic_simplified(refManager);
  this->radio_surf_glob = new Surfacic_global_piecewise(refopeManager);
  this->radio_surf_local = new Surfacic_local(refopeManager);
  this->radio_surf_seg = new Surfacic_segmented(refopeManager);
  this->radio_sepa_glob = new Separation_global(refManager);
  this->radio_sepa_local = new Separation_local(refopeManager);

  //Attributs
  this->algoSelected = 0;
  plotManager->set_ticSize(15); //Default: 10

  //---------------------------
}
Radiometry::~Radiometry(){}

//Correction Methods
void Radiometry::algo_RadarEquation(int num){
  /*Cloud* cloud = sceneManager->get_selected_cloud();
  attribManager->compute_attribut_cloud(cloud);
  //---------------------------

  switch(num){
    case 0: radio_radar->compute_RadarEquation(cloud); break;
    case 1: radio_radar->compute_IR2bycosIt(cloud); break;
    case 2: radio_radar->compute_IbyCosIt(cloud); break;
    case 3: radio_radar->compute_IbyR2(cloud); break;
  }

  //---------------------------
  cloud->intensity.OBJ = radio_radar->get_Ic();
  cloud->intensity.heatmap = false;*/
}
void Radiometry::algo_surfacicSimplified(Cloud* cloud){
  refManager->load_SphereTarget_precomp_80d();
  //---------------------------

  //Algorithm
  if(check_cloudData(cloud) == false)
  if(radio_surf_simpl->algo_Bretagne(cloud)){
    Ic = radio_surf_simpl->get_Ic();
    Im = radio_surf_simpl->get_Im();
  }

  //---------------------------
}
void Radiometry::algo_surfacicGlobalPiecewise(Cloud* cloud){
  refManager->load_SphereTarget_precomp_80d();
  //---------------------------

  //Compute cloud attributs
  attribManager->compute_attribut_cloud(cloud);

  //Algorithm
  if(radio_surf_glob->algo(cloud)){
    Ic = radio_surf_glob->get_Ic();
    Im = radio_surf_glob->get_Im();
  }

  //---------------------------
}
void Radiometry::algo_surfacicLocal(Cloud* cloud){
  refManager->load_SphereTarget_precomp_80d();
  //---------------------------

  //Compute cloud attributs
  attribManager->compute_attribut_cloud(cloud);

  //Check cloud data
  if(radio_surf_local->algo(cloud)){
    Ic = radio_surf_local->get_Ic();
    Im = radio_surf_local->get_Im();
  }

  //---------------------------
}
void Radiometry::algo_surfacicSegmented(Cloud* cloud){
  refManager->load_SphereTarget_precomp_add80d();
  //---------------------------

  //Compute cloud attributs
  attribManager->compute_attribut_cloud(cloud);

  //Check cloud data
  radio_surf_seg->algo(cloud);
  Ic = radio_surf_seg->get_Ic();
  Im = radio_surf_seg->get_Im();


  //---------------------------
}
void Radiometry::algo_separationGlobal(Cloud* cloud){
  refManager->load_SpectralonTarget();
  //---------------------------

  //Compute cloud attributs
  attribManager->compute_attribut_cloud(cloud);

  //Algorithm
  if(radio_sepa_glob->algo(cloud)){
    Ic = radio_sepa_glob->get_Ic();
    Im = radio_sepa_glob->get_Im();
  }

  //---------------------------
}

//Correction functions
void Radiometry::correction_allClouds(){
  /*list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Correct all cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    //Correction
    this->compute_RadioCorrection(cloud);

    //Update
    if(cloud->intensity.Buffer.size() != 0){
      cloud->intensity.OBJ = cloud->intensity.Buffer;
      cloud->intensity.Buffer.clear();
    }
  }*/

  //---------------------------
}
void Radiometry::correction_allClouds_Iini(){
  /*list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Correct all cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    cloud->location.Buffer = cloud->location.OBJ;
    cloud->normal.Buffer = cloud->normal.OBJ;

    cloud->location.OBJ = cloud->location.Initial;
    cloud->normal.OBJ = cloud->normal.Initial;

    this->compute_RadioCorrection(cloud);
  }

  //Update all cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    cloud->location.OBJ = cloud->location.Buffer;
    cloud->normal.OBJ = cloud->normal.Buffer;

    //For avoid heatmap (?)
    if(cloud->intensity.Buffer.size() != 0){
      cloud->intensity.OBJ = cloud->intensity.Buffer;
      cloud->intensity.Buffer.clear();
    }
  }*/

  //---------------------------
}
void Radiometry::correction_allClouds_WtRefs(){
  /*list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  Is_std.clear(); Is_CV.clear(); Ic_std.clear(); Ic_CV.clear();
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    if(cloud->name.find("Sphere") == std::string::npos && cloud->name.find("Spectralon") == std::string::npos){
      this->correction_oneCloud(cloud);
    }
  }

  //---------------------------
  this->wrt_results();*/
}
void Radiometry::correction_oneCloud(Cloud* cloud){
  //---------------------------

  this->compute_RadioCorrection(cloud);
  //cloud->intensity.OBJ = cloud->intensity.Buffer;

  //---------------------------
}
void Radiometry::correction_oneCloud_Iini(Cloud* cloud){
  /*cloud->location.Buffer = cloud->location.OBJ;
  cloud->normal.Buffer = cloud->normal.OBJ;
  cloud->location.OBJ = cloud->location.Initial;
  cloud->normal.OBJ = cloud->normal.Initial;
  //---------------------------

  this->compute_RadioCorrection(cloud);
  cloud->intensity.OBJ = cloud->intensity.Buffer;

  //---------------------------
  cloud->location.OBJ = cloud->location.Buffer;
  cloud->normal.OBJ = cloud->normal.Buffer;*/
}
bool Radiometry::compute_RadioCorrection(Cloud* cloud){
  /*Ic.clear(); Im.clear();
  vector<float>& Is = cloud->intensity.OBJ;
  //---------------------------

  //Correction algorithm
  switch(algoSelected){
    case 0: this->algo_surfacicSegmented(cloud); break;
    case 1: this->algo_surfacicGlobalPiecewise(cloud); break;
    case 2: this->algo_separationGlobal(cloud); break;
    case 3: this->algo_surfacicLocal(cloud); break;
    case 4: this->algo_surfacicSimplified(cloud); break;
    case 5: this->algo_RadarEquation(0); break;
  }

  //If well functionning
  if(Ic.size() !=0 && Im.size() != 0){
    //Result
    console.AddLog("%s corrected -> Iraw: mean = %.2f, CV = %.2f | Icor: mean = %.2f, CV = %.2f",
    cloud->name.c_str(), fct_mean(Is), fct_cv(Is), fct_mean(Ic), fct_cv(Ic));

    //Parameters
    cloud->intensity.heatmap = false;
    cloud->intensity.corrected = true;
    cloud->intensity.OBJ = Ic;
    cloud->intensity.Buffer = Ic;

    //Update color
    sceneManager->update_cloud_IntensityToColor(cloud);
  }else{
    console.AddLog("[error] Problem occurs during correction");
  }

  //---------------------------*/
  return true;
}

//Subfunctions
void Radiometry::compute_sortByIt(vector<float>& vec1, vector<float>& vec2){
  //---------------------------

  vector<size_t> idx = fct_sortByIndexes(vec1);
  sort(vec1.begin(), vec1.end());
  vector<float> tempo;
  for(int i=0; i<vec2.size(); i++){
    tempo.push_back(vec2[idx[i]]);
  }

  //---------------------------
  vec2 = tempo;
}
void Radiometry::compute_SpectralonAjustement(){
  /*list<Cloud*>* list_99p = refManager->get_list_99p_40m_xd();
  list<Cloud*>* list_50p = refManager->get_list_50p_40m_xd();
  list<Cloud*>* list_25p = refManager->get_list_25p_40m_xd();
  list<Cloud*>* list_10p = refManager->get_list_10p_40m_xd();
  //---------------------------

  for(int i=0; i<list_99p->size() ;i++){
    Cloud* cloud_99 = *next(list_99p->begin(),i);
    Cloud* cloud_50 = *next(list_50p->begin(),i);
    Cloud* cloud_25 = *next(list_25p->begin(),i);
    Cloud* cloud_10 = *next(list_10p->begin(),i);

    vector<float>& Is_99 = cloud_99->intensity.OBJ;
    vector<float>& It_99 = cloud_99->attribut.It;
    vector<float>& Is_50 = cloud_50->intensity.OBJ;
    vector<float>& Is_25 = cloud_25->intensity.OBJ;
    vector<float>& Is_10 = cloud_10->intensity.OBJ;

    //low angle reducer
    for(int i=0; i<It_99.size(); i++){
      if(It_99[i]<5){
        Is_99[i] = Is_99[i] + 0.105;
        if(Is_99[i] >= 0.99) Is_99[i] = 0.996338;
        Is_50[i] = Is_50[i] + 0.105;
        Is_25[i] = Is_25[i] + 0.105;
        Is_10[i] = Is_10[i] + 0.105;
      }
    }
  }*/

  //---------------------------
}
void Radiometry::set_referenceON(bool value){
  if(refManager->is_listsCompiled() == false){
    refManager->compute_list(sceneManager->get_list_cloud());
  }
  list<Cloud*>* list = refManager->get_listReference();
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);
    cloud->visibility = value;
  }

  //---------------------------
  cout<<"Reference targets visibility: "<<value<<endl;
}
void Radiometry::wrt_results(){
  ofstream corrResults;
  //---------------------------

  corrResults.open ("../../../../../../ICorrection.txt");
  corrResults << "std(Is)"<<" "<<"std(Ic)"<<" "<<setprecision(3)<<"CV(Is)"<<" "<<"CV(Ic)"<<"\n";
  corrResults << "--------------------------------"<<"\n";
  for(int i=0; i<Is_std.size(); i++){
    corrResults << Is_std[i]<<" "<<Ic_std[i]<<" "<<Is_CV[i]<<" "<<Ic_CV[i]<<"\n";
  }

  //---------------------------
  corrResults.close();
}
void Radiometry::remove_References(){
  if(refManager->is_listsCompiled() == false){
    refManager->compute_list(sceneManager->get_list_cloud());
  }
  list<Cloud*>* list = refManager->get_listReference();
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);
    sceneManager->remove_cloud(cloud);
  }

  //---------------------------
  refManager->clear();
}
bool Radiometry::check_cloudData(Cloud* cloud){
  /*attribManager->compute_attribut_cloud(cloud);
  //---------------------------

  if(cloud->intensity.hasData == false){
    cout<<"Correction -> No intensity data"<<endl;
    return false;
  }
  if(cloud->normal.hasData == false){
    cout<<"Correction -> No normal data"<<endl;
    return false;
  }
  if(cloud->attribut.dist.size() == 0){
    cout<<"Correction -> No distance data"<<endl;
    return false;
  }
  if(cloud->attribut.cosIt.size() == 0){
    cout<<"Correction -> No cosIt data"<<endl;
    return false;
  }
  if(cloud->intensity.corrected == true){
    cout<<"Correction -> Cloud already corrected"<<endl;
    return false;
  }
  if(cloud->location.OBJ.size() != cloud->intensity.OBJ.size()){
    cout<<"Correction -> Sizing problem"<<endl;
    return false;
  }*/

  //---------------------------
  return true;
}
