#include "Ref_Operation.h"
#include "Reference.h"

#include "../../../../Operation/Function/BundleByClass.h"
#include "../../../../Operation/Optimization/Polyfit.h"
#include "../../../../Specific/fct_math.h"


//Constructor / Destructor
Ref_Operation::Ref_Operation(Reference* refClass){
  this->refManager = refClass;
  //---------------------------

  bundler = new BundleByClass();

  //---------------------------
}
Ref_Operation::~Ref_Operation(){}

//Reference operation
bool Ref_Operation::compute_Sphere_IbyR(){
  if(!refManager->isref_Sphere()){
    cout<<"No Sphere ref"<<endl;
    return false;
  }
  list<Cloud*>* list = refManager->get_listSphere();
  Sphere_IbyR_I01.clear();
  Sphere_IbyR_R.clear();
  Sphere_IbyR_I01_std.clear();
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);

    bundler->compute_bundleByClass_maxAngle(cloud, 5, 17);
    vector<float> Ib_01 = bundler->get_Ib();
    vector<float> Ib_R = bundler->get_Ib_dist();

    Sphere_IbyR_I01.push_back(fct_mean(Ib_01));
    Sphere_IbyR_R.push_back(fct_mean(Ib_R));
    Sphere_IbyR_I01_std.push_back(fct_std(Ib_01));
  }

  //---------------------------
  return true;
}
bool Ref_Operation::compute_Sphere_IbycIt(){
  if(!refManager->isref_Sphere()){
    cout<<"No Sphere ref"<<endl;
    return false;
  }
  list<Cloud*>* list = refManager->get_listSphere();
  Sphere_IbycIt_cIt.clear(); Sphere_IbycIt_I.clear();
  Sphere_IbycIt_std.clear(); Sphere_IbycIt_It.clear();
  Sphere_IbycIt_R.clear();
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);

    bundler->compute_bundleByClass(cloud, 10);
    vector<float> Ib = bundler->get_Ib();
    vector<float> Ib_std = bundler->get_Ib_std();
    vector<float> Ib_cosIt = bundler->get_Ib_cosIt();
    vector<float> Ib_It = bundler->get_Ib_It();
    vector<float> Ib_dist = bundler->get_Ib_dist();

    Sphere_IbycIt_R.push_back(Ib_dist);
    Sphere_IbycIt_I.push_back(Ib);
    Sphere_IbycIt_std.push_back(Ib_std);
    Sphere_IbycIt_It.push_back(Ib_It);
    Sphere_IbycIt_cIt.push_back(Ib_cosIt);
  }

  //---------------------------
  if(Sphere_IbycIt_I.size() == 0 || Sphere_IbycIt_cIt.size() == 0){
    cout<<"Problem calculing Sphere IbyR"<<endl;
  }
  return true;
}
vector<vec3> Ref_Operation::get_sphereData(int i){
  list<Cloud*>* list = refManager->get_listSphere();
  vector<vec3> sphere_data;
  //---------------------------

  Cloud* sphere = *next(list->begin(),i);

  bundler->compute_bundleByClass(sphere, 2);
  vector<float> Ib_01 = bundler->get_Ib();
  vector<float> Ib_R = bundler->get_Ib_dist();
  vector<float> Ib_cIt = bundler->get_Ib_cosIt();

  for(int j=0; j<Ib_01.size(); j++){
    vec3 data = vec3(Ib_R[j], Ib_cIt[j], Ib_01[j]);
    sphere_data.push_back(data);
  }

  //---------------------------
  return sphere_data;
}

bool Ref_Operation::compute_Surfacic_gloabPiecewise(float R_1, float R_2){
  if(!refManager->isref_Sphere()){
    cout<<"No Sphere ref"<<endl;
    return false;
  }
  list<Cloud*>* list = refManager->get_listSphere();
  Pfeifer_cIt_b.clear(); Pfeifer_cIt_m.clear(); Pfeifer_cIt_e.clear();
  Pfeifer_I01_b.clear(); Pfeifer_I01_m.clear(); Pfeifer_I01_e.clear();
  Pfeifer_R_b.clear(); Pfeifer_R_m.clear(); Pfeifer_R_e.clear();
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);

    bundler->compute_bundleByClass(cloud, 5);
    vector<float> Ib = bundler->get_Ib();
    vector<float> Ib_cIt = bundler->get_Ib_cosIt();
    vector<float> Ib_R = bundler->get_Ib_dist();

    float R = fct_mean(Ib_R);
    if(R <= R_1){
      Pfeifer_I01_b.push_back(Ib);
      Pfeifer_cIt_b.push_back(Ib_cIt);
      Pfeifer_R_b.push_back(Ib_R);
    }else if(R > R_1 && R <= R_2){
      Pfeifer_I01_m.push_back(Ib);
      Pfeifer_cIt_m.push_back(Ib_cIt);
      Pfeifer_R_m.push_back(Ib_R);
    }else if(R > R_2){
      Pfeifer_I01_e.push_back(Ib);
      Pfeifer_cIt_e.push_back(Ib_cIt);
      Pfeifer_R_e.push_back(Ib_R);
    }
  }

  //---------------------------
  return true;
}
bool Ref_Operation::compute_ParameterSpace_Sphere(float bundle){
  if(!refManager->isref_Sphere()){
    cout<<"--> Parameter space: Do not find sphere references"<<endl;
    return false;
  }
  list<Cloud*>* list_ref = refManager->get_listSphere();
  PS.clear();
  //---------------------------

  //Parameter space
  for(int i=0; i<list_ref->size(); i++){
    Cloud* cloud = *next(list_ref->begin(),i);

    bundler->compute_bundleByClass(cloud, bundle);
    vector<float> Ib = bundler->get_Ib();
    vector<float> Ib_cosIt = bundler->get_Ib_cosIt();
    vector<float> Ib_dist = bundler->get_Ib_dist();

    for(int j=0; j<Ib.size(); j++){
      vec3 point = vec3(Ib[j], Ib_cosIt[j], Ib_dist[j]);
      PS.push_back(point);
    }
  }

  //Write data on file
  if(list_ref->size() != 0){
    ofstream file;
    file.open ("../../data/data/PS_Sphere.txt");
    file << "Iraw[0;1] "<<"R "<<"It "<<"\n";
    file << "--------------------------------"<<"\n";
    file <<std::fixed;
    for(int i=0; i<PS.size(); i++){
      file <<setprecision(5)<< PS[i].x<<" "<<PS[i].z<<" "<<acos(PS[i].y) * 180 / M_PI<<"\n";
    }
    file.close();
  }

  //---------------------------
  if(PS.size() == 0){
    cout<<"--> Parameter space: failed"<<endl;
    return false;
  }
  return true;
}
bool Ref_Operation::compute_ParameterSpace_Spectralon(){
  /*if(!refManager->isref_Spectralon()){
    return false;
  }
  list<Cloud*>* list_ref = refManager->get_listSpectralon();
  PS.clear();
  //---------------------------

  //Parameter space
  for(int i=0; i<list_ref->size(); i++){
    Cloud* cloud = *next(list_ref->begin(),i);

    float Ib = fct_mean(cloud->intensity.OBJ);
    float Ib_cosIt = fct_mean(cloud->attribut.cosIt);
    float Ib_dist = fct_mean(cloud->attribut.dist);

    vec3 point = vec3(Ib, Ib_cosIt, Ib_dist);
    PS.push_back(point);
  }

  //Write data on file
  if(list_ref->size() != 0){
    ofstream file;
    file.open ("../../data/data/PS_Spectralon.txt");
    file << "Iraw[0;1] "<<"R "<<"It "<<"rho "<<"\n";
    file << "--------------------------------"<<"\n";

    //------------
    this->compute_Spectralon_IbyR();
    vector<float> I_99 = this->get_Spec_IbyR_I01_99();
    vector<float> I_50 = this->get_Spec_IbyR_I01_50();
    vector<float> I_25 = this->get_Spec_IbyR_I01_25();
    vector<float> I_10 = this->get_Spec_IbyR_I01_10();

    vector<float> R_99 = this->get_Spec_IbyR_R_99();
    vector<float> R_50 = this->get_Spec_IbyR_R_50();
    vector<float> R_25 = this->get_Spec_IbyR_R_25();
    vector<float> R_10 = this->get_Spec_IbyR_R_10();

    vector<float> It_99 = this->get_Spec_IbyR_It_99();
    vector<float> It_50 = this->get_Spec_IbyR_It_50();
    vector<float> It_25 = this->get_Spec_IbyR_It_25();
    vector<float> It_10 = this->get_Spec_IbyR_It_10();

    file <<std::fixed;
    for(int i=0; i<I_99.size(); i++){
      file <<setprecision(5)<< I_99[i]<<" "<<R_99[i]<<" "<<It_99[i]<<" "<<99<<"\n";
      file <<setprecision(5)<< I_50[i]<<" "<<R_50[i]<<" "<<It_50[i]<<" "<<57<<"\n";
      file <<setprecision(5)<< I_25[i]<<" "<<R_25[i]<<" "<<It_25[i]<<" "<<28<<"\n";
      file <<setprecision(5)<< I_10[i]<<" "<<R_10[i]<<" "<<It_10[i]<<" "<<11<<"\n";
    }
    //------------

    //------------
    this->compute_Spectralon_IbycIt(10);
    I_99 = this->get_Spec_IbycIt_I01_99();
    I_50 = this->get_Spec_IbycIt_I01_50();
    I_25 = this->get_Spec_IbycIt_I01_25();
    I_10 = this->get_Spec_IbycIt_I01_10();

    R_99 = this->get_Spec_IbycIt_R_99();
    R_50 = this->get_Spec_IbycIt_R_50();
    R_25 = this->get_Spec_IbycIt_R_25();
    R_10 = this->get_Spec_IbycIt_R_10();

    It_99 = this->get_Spec_IbycIt_It_99();
    It_50 = this->get_Spec_IbycIt_It_50();
    It_25 = this->get_Spec_IbycIt_It_25();
    It_10 = this->get_Spec_IbycIt_It_10();

    file <<std::fixed;
    for(int i=0; i<I_99.size(); i++){
      file <<setprecision(5)<< I_99[i]<<" "<<R_99[i]<<" "<<It_99[i]<<" "<<99<<"\n";
      file <<setprecision(5)<< I_50[i]<<" "<<R_50[i]<<" "<<It_50[i]<<" "<<57<<"\n";
      file <<setprecision(5)<< I_25[i]<<" "<<R_25[i]<<" "<<It_25[i]<<" "<<28<<"\n";
      file <<setprecision(5)<< I_10[i]<<" "<<R_10[i]<<" "<<It_10[i]<<" "<<11<<"\n";
    }
    //------------

    //------------
    this->compute_Spectralon_IbycIt(20);
    I_99 = this->get_Spec_IbycIt_I01_99();
    I_50 = this->get_Spec_IbycIt_I01_50();
    I_25 = this->get_Spec_IbycIt_I01_25();
    I_10 = this->get_Spec_IbycIt_I01_10();

    R_99 = this->get_Spec_IbycIt_R_99();
    R_50 = this->get_Spec_IbycIt_R_50();
    R_25 = this->get_Spec_IbycIt_R_25();
    R_10 = this->get_Spec_IbycIt_R_10();

    It_99 = this->get_Spec_IbycIt_It_99();
    It_50 = this->get_Spec_IbycIt_It_50();
    It_25 = this->get_Spec_IbycIt_It_25();
    It_10 = this->get_Spec_IbycIt_It_10();

    file <<std::fixed;
    for(int i=0; i<I_99.size(); i++){
      file <<setprecision(5)<< I_99[i]<<" "<<R_99[i]<<" "<<It_99[i]<<" "<<99<<"\n";
      file <<setprecision(5)<< I_50[i]<<" "<<R_50[i]<<" "<<It_50[i]<<" "<<57<<"\n";
      file <<setprecision(5)<< I_25[i]<<" "<<R_25[i]<<" "<<It_25[i]<<" "<<28<<"\n";
      file <<setprecision(5)<< I_10[i]<<" "<<R_10[i]<<" "<<It_10[i]<<" "<<11<<"\n";
    }
    //------------

    //------------
    this->compute_Spectralon_IbycIt(30);
    I_99 = this->get_Spec_IbycIt_I01_99();
    I_50 = this->get_Spec_IbycIt_I01_50();
    I_25 = this->get_Spec_IbycIt_I01_25();
    I_10 = this->get_Spec_IbycIt_I01_10();

    R_99 = this->get_Spec_IbycIt_R_99();
    R_50 = this->get_Spec_IbycIt_R_50();
    R_25 = this->get_Spec_IbycIt_R_25();
    R_10 = this->get_Spec_IbycIt_R_10();

    It_99 = this->get_Spec_IbycIt_It_99();
    It_50 = this->get_Spec_IbycIt_It_50();
    It_25 = this->get_Spec_IbycIt_It_25();
    It_10 = this->get_Spec_IbycIt_It_10();

    file <<std::fixed;
    for(int i=0; i<I_99.size(); i++){
      file <<setprecision(5)<< I_99[i]<<" "<<R_99[i]<<" "<<It_99[i]<<" "<<99<<"\n";
      file <<setprecision(5)<< I_50[i]<<" "<<R_50[i]<<" "<<It_50[i]<<" "<<57<<"\n";
      file <<setprecision(5)<< I_25[i]<<" "<<R_25[i]<<" "<<It_25[i]<<" "<<28<<"\n";
      file <<setprecision(5)<< I_10[i]<<" "<<R_10[i]<<" "<<It_10[i]<<" "<<11<<"\n";
    }
    //------------

    //------------
    this->compute_Spectralon_IbycIt(40);
    I_99 = this->get_Spec_IbycIt_I01_99();
    I_50 = this->get_Spec_IbycIt_I01_50();
    I_25 = this->get_Spec_IbycIt_I01_25();
    I_10 = this->get_Spec_IbycIt_I01_10();

    R_99 = this->get_Spec_IbycIt_R_99();
    R_50 = this->get_Spec_IbycIt_R_50();
    R_25 = this->get_Spec_IbycIt_R_25();
    R_10 = this->get_Spec_IbycIt_R_10();

    It_99 = this->get_Spec_IbycIt_It_99();
    It_50 = this->get_Spec_IbycIt_It_50();
    It_25 = this->get_Spec_IbycIt_It_25();
    It_10 = this->get_Spec_IbycIt_It_10();

    file <<std::fixed;
    for(int i=0; i<I_99.size(); i++){
      file <<setprecision(5)<< I_99[i]<<" "<<R_99[i]<<" "<<It_99[i]<<" "<<99<<"\n";
      file <<setprecision(5)<< I_50[i]<<" "<<R_50[i]<<" "<<It_50[i]<<" "<<57<<"\n";
      file <<setprecision(5)<< I_25[i]<<" "<<R_25[i]<<" "<<It_25[i]<<" "<<28<<"\n";
      file <<setprecision(5)<< I_10[i]<<" "<<R_10[i]<<" "<<It_10[i]<<" "<<11<<"\n";
    }
    //------------
    file.close();
  }

  //---------------------------*/
  return true;
}
bool Ref_Operation::compute_Spectralon_IbyR(){
  /*Spec_IbyR_I01_99.clear();
  Spec_IbyR_I01_50.clear();
  Spec_IbyR_I01_25.clear();
  Spec_IbyR_I01_10.clear();

  Spec_IbyR_I2048_99.clear();
  Spec_IbyR_I2048_50.clear();
  Spec_IbyR_I2048_25.clear();
  Spec_IbyR_I2048_10.clear();

  Spec_IbyR_std_99.clear();
  Spec_IbyR_std_50.clear();
  Spec_IbyR_std_25.clear();
  Spec_IbyR_std_10.clear();

  Spec_IbyR_R_99.clear();
  Spec_IbyR_R_50.clear();
  Spec_IbyR_R_25.clear();
  Spec_IbyR_R_10.clear();

  Spec_IbyR_It_99.clear();
  Spec_IbyR_It_50.clear();
  Spec_IbyR_It_25.clear();
  Spec_IbyR_It_10.clear();

  Spec_IbyR_nb_99.clear();
  Spec_IbyR_nb_50.clear();
  Spec_IbyR_nb_25.clear();
  Spec_IbyR_nb_10.clear();
  //---------------------------

  //Get lists
  if(!refManager->isref_Spectralon()){
    refManager->load_SpectralonTarget();
  }
  list<Cloud*>* list_99p_xm = refManager->get_list_99p_xm();
  list<Cloud*>* list_50p_xm = refManager->get_list_50p_xm();
  list<Cloud*>* list_25p_xm = refManager->get_list_25p_xm();
  list<Cloud*>* list_10p_xm = refManager->get_list_10p_xm();

  for(int i=0; i<list_99p_xm->size() ;i++){
    Cloud* cloud_99 = *next(list_99p_xm->begin(),i);
    Cloud* cloud_50 = *next(list_50p_xm->begin(),i);
    Cloud* cloud_25 = *next(list_25p_xm->begin(),i);
    Cloud* cloud_10 = *next(list_10p_xm->begin(),i);

    Spec_IbyR_I01_99.push_back(fct_mean(cloud_99->intensity.OBJ));
    Spec_IbyR_I01_50.push_back(fct_mean(cloud_50->intensity.OBJ));
    Spec_IbyR_I01_25.push_back(fct_mean(cloud_25->intensity.OBJ));
    Spec_IbyR_I01_10.push_back(fct_mean(cloud_10->intensity.OBJ));

    Spec_IbyR_I2048_99.push_back(fct_mean(cloud_99->intensity.OBJ) * 2048);
    Spec_IbyR_I2048_50.push_back(fct_mean(cloud_50->intensity.OBJ) * 2048);
    Spec_IbyR_I2048_25.push_back(fct_mean(cloud_25->intensity.OBJ) * 2048);
    Spec_IbyR_I2048_10.push_back(fct_mean(cloud_10->intensity.OBJ) * 2048);

    Spec_IbyR_R_99.push_back(fct_mean(cloud_99->attribut.dist));
    Spec_IbyR_R_50.push_back(fct_mean(cloud_50->attribut.dist));
    Spec_IbyR_R_25.push_back(fct_mean(cloud_25->attribut.dist));
    Spec_IbyR_R_10.push_back(fct_mean(cloud_10->attribut.dist));

    Spec_IbyR_std_99.push_back(fct_std(cloud_99->intensity.OBJ));
    Spec_IbyR_std_50.push_back(fct_std(cloud_50->intensity.OBJ));
    Spec_IbyR_std_25.push_back(fct_std(cloud_25->intensity.OBJ));
    Spec_IbyR_std_10.push_back(fct_std(cloud_10->intensity.OBJ));

    Spec_IbyR_It_99.push_back(fct_mean(cloud_99->attribut.It));
    Spec_IbyR_It_50.push_back(fct_mean(cloud_50->attribut.It));
    Spec_IbyR_It_25.push_back(fct_mean(cloud_25->attribut.It));
    Spec_IbyR_It_10.push_back(fct_mean(cloud_10->attribut.It));

    Spec_IbyR_nb_99.push_back(cloud_99->intensity.OBJ.size());
    Spec_IbyR_nb_50.push_back(cloud_50->intensity.OBJ.size());
    Spec_IbyR_nb_25.push_back(cloud_25->intensity.OBJ.size());
    Spec_IbyR_nb_10.push_back(cloud_10->intensity.OBJ.size());
  }

  //---------------------------
  if(Spec_IbyR_I01_99.size() == 0 || Spec_IbyR_R_99.size() == 0){
    cout<<"Problem calculing Spectralon IbyR "<< Spec_IbyR_I01_99.size()<<" "<<Spec_IbyR_R_99.size()<<endl;
  }*/
  return true;
}
bool Ref_Operation::compute_Spectralon_IbycIt(int distance){
  Spec_IbycIt_I01_99.clear(); Spec_IbycIt_std_99.clear();
  Spec_IbycIt_I01_50.clear(); Spec_IbycIt_std_50.clear();
  Spec_IbycIt_I01_25.clear(); Spec_IbycIt_std_25.clear();
  Spec_IbycIt_I01_10.clear(); Spec_IbycIt_std_10.clear();

  Spec_IbycIt_cIt_99.clear(); Spec_IbycIt_It_99.clear();
  Spec_IbycIt_cIt_50.clear(); Spec_IbycIt_It_50.clear();
  Spec_IbycIt_cIt_25.clear(); Spec_IbycIt_It_25.clear();
  Spec_IbycIt_cIt_10.clear(); Spec_IbycIt_It_10.clear();

  Spec_IbycIt_R_99.clear(); Spec_IbycIt_nb_99.clear();
  Spec_IbycIt_R_50.clear(); Spec_IbycIt_nb_50.clear();
  Spec_IbycIt_R_25.clear(); Spec_IbycIt_nb_25.clear();
  Spec_IbycIt_R_10.clear(); Spec_IbycIt_nb_10.clear();
  //---------------------------

  //Get Spectralon lists
  list<Cloud*>* list_99p = new list<Cloud*>;
  list<Cloud*>* list_50p = new list<Cloud*>;
  list<Cloud*>* list_25p = new list<Cloud*>;
  list<Cloud*>* list_10p = new list<Cloud*>;
  if(!refManager->isref_Spectralon()){
    cout<<"No Spectralon ref"<<endl;
    return false;
  }
  switch (distance) {
    case 05:{
      list_99p = refManager->get_list_99p_05m_xd();
      list_50p = refManager->get_list_50p_05m_xd();
      list_25p = refManager->get_list_25p_05m_xd();
      list_10p = refManager->get_list_10p_05m_xd();
      break;
    }
    case 10:{
      list_99p = refManager->get_list_99p_10m_xd();
      list_50p = refManager->get_list_50p_10m_xd();
      list_25p = refManager->get_list_25p_10m_xd();
      list_10p = refManager->get_list_10p_10m_xd();
      break;
    }
    case 20:{
      list_99p = refManager->get_list_99p_20m_xd();
      list_50p = refManager->get_list_50p_20m_xd();
      list_25p = refManager->get_list_25p_20m_xd();
      list_10p = refManager->get_list_10p_20m_xd();
      break;
    }
    case 30:{
      list_99p = refManager->get_list_99p_30m_xd();
      list_50p = refManager->get_list_50p_30m_xd();
      list_25p = refManager->get_list_25p_30m_xd();
      list_10p = refManager->get_list_10p_30m_xd();
      break;
    }
    case 40:{
      list_99p = refManager->get_list_99p_40m_xd();
      list_50p = refManager->get_list_50p_40m_xd();
      list_25p = refManager->get_list_25p_40m_xd();
      list_10p = refManager->get_list_10p_40m_xd();
      break;
    }
    default:{
      cout<<"No good distance"<<endl;
      return false;
    }
  }
  if(list_99p->size() == 0){
    return false;
  }

  for(int i=0; i<list_99p->size() ;i++){
    /*Cloud* cloud_99 = *next(list_99p->begin(),i);
    Cloud* cloud_50 = *next(list_50p->begin(),i);
    Cloud* cloud_25 = *next(list_25p->begin(),i);
    Cloud* cloud_10 = *next(list_10p->begin(),i);

    // 99%
    vector<float>& Is_99 = cloud_99->intensity.OBJ;
    vector<float>& It_99 = cloud_99->attribut.It;
    vector<float>& cIt_99 = cloud_99->attribut.cosIt;
    vector<float>& R_99 = cloud_99->attribut.dist;

    Spec_IbycIt_I01_99.push_back(fct_mean(Is_99));
    Spec_IbycIt_It_99.push_back(fct_mean(It_99));
    Spec_IbycIt_cIt_99.push_back(fct_mean(cIt_99));
    Spec_IbycIt_std_99.push_back(fct_std(Is_99));
    Spec_IbycIt_R_99.push_back(fct_mean(R_99));
    Spec_IbycIt_nb_99.push_back(Is_99.size());
    //--------------------------------

    // 50%
    vector<float>& Is_50 = cloud_50->intensity.OBJ;
    vector<float>& It_50 = cloud_50->attribut.It;
    vector<float>& cIt_50 = cloud_50->attribut.cosIt;
    vector<float>& R_50 = cloud_50->attribut.dist;

    Spec_IbycIt_I01_50.push_back(fct_mean(Is_50));
    Spec_IbycIt_It_50.push_back(fct_mean(It_50));
    Spec_IbycIt_cIt_50.push_back(fct_mean(cIt_50));
    Spec_IbycIt_std_50.push_back(fct_std(Is_50));
    Spec_IbycIt_R_50.push_back(fct_mean(R_50));
    Spec_IbycIt_nb_50.push_back(Is_50.size());
    //--------------------------------

    // 25%
    vector<float>& Is_25 = cloud_25->intensity.OBJ;
    vector<float>& It_25 = cloud_25->attribut.It;
    vector<float>& cIt_25 = cloud_25->attribut.cosIt;
    vector<float>& R_25 = cloud_25->attribut.dist;

    Spec_IbycIt_I01_25.push_back(fct_mean(Is_25));
    Spec_IbycIt_It_25.push_back(fct_mean(It_25));
    Spec_IbycIt_cIt_25.push_back(fct_mean(cIt_25));
    Spec_IbycIt_std_25.push_back(fct_std(Is_25));
    Spec_IbycIt_R_25.push_back(fct_mean(R_25));
    Spec_IbycIt_nb_25.push_back(Is_25.size());
    //--------------------------------

    // 10%
    vector<float>& Is_10 = cloud_10->intensity.OBJ;
    vector<float>& It_10 = cloud_10->attribut.It;
    vector<float>& cIt_10 = cloud_10->attribut.cosIt;
    vector<float>& R_10 = cloud_10->attribut.dist;

    Spec_IbycIt_I01_10.push_back(fct_mean(Is_10));
    Spec_IbycIt_It_10.push_back(fct_mean(It_10));
    Spec_IbycIt_cIt_10.push_back(fct_mean(cIt_10));
    Spec_IbycIt_std_10.push_back(fct_std(Is_10));
    Spec_IbycIt_R_10.push_back(fct_mean(R_10));
    Spec_IbycIt_nb_10.push_back(Is_10.size());*/
    //--------------------------------
  }

  //---------------------------
  return true;
}
void Ref_Operation::compute_sortByIt(vector<float>& vec1, vector<float>& vec2, vector<float>& vec3){
  //---------------------------

  vector<size_t> idx = fct_sortByIndexes(vec1);
  sort(vec1.begin(), vec1.end());
  vector<float> temp1, temp2;
  for(int i=0; i<vec2.size(); i++){
    temp1.push_back(vec2[idx[i]]);
    temp2.push_back(vec3[idx[i]]);
  }

  //---------------------------
  vec2 = temp1;
  vec3 = temp2;
}
