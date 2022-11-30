#include "Reference.h"

#include "../../../../Engine/Node_engine.h"
#include "../../../../Operation/Node_operation.h"
#include "../../../../Operation/Transformation/Attribut.h"
#include "../../../../Load/Node_load.h"
#include "../../../../Load/Processing/Loader.h"


//Constructor / Destructor
Reference::Reference(Node_engine* node){
  //---------------------------

  Node_load* node_load = node->get_node_load();
  Node_operation* node_ope = node->get_node_ope();

  this->loaderManager = node_load->get_loaderManager();
  this->attribManager = new Attribut(node_ope);

  this->list_Sphere = new list<Cloud*>;
  this->list_Spectralon = new list<Cloud*>;

  this->list_10p_xm_xd = new list<Cloud*>;
  this->list_25p_xm_xd = new list<Cloud*>;
  this->list_50p_xm_xd = new list<Cloud*>;
  this->list_99p_xm_xd = new list<Cloud*>;

  this->list_xp_10m_xd = new list<Cloud*>;
  this->list_xp_20m_xd = new list<Cloud*>;
  this->list_xp_30m_xd = new list<Cloud*>;
  this->list_xp_40m_xd = new list<Cloud*>;

  this->list_10p_05m_xd = new list<Cloud*>;
  this->list_25p_05m_xd = new list<Cloud*>;
  this->list_50p_05m_xd = new list<Cloud*>;
  this->list_99p_05m_xd = new list<Cloud*>;

  this->list_10p_10m_xd = new list<Cloud*>;
  this->list_25p_10m_xd = new list<Cloud*>;
  this->list_50p_10m_xd = new list<Cloud*>;
  this->list_99p_10m_xd = new list<Cloud*>;

  this->list_10p_20m_xd = new list<Cloud*>;
  this->list_25p_20m_xd = new list<Cloud*>;
  this->list_50p_20m_xd = new list<Cloud*>;
  this->list_99p_20m_xd = new list<Cloud*>;

  this->list_10p_30m_xd = new list<Cloud*>;
  this->list_25p_30m_xd = new list<Cloud*>;
  this->list_50p_30m_xd = new list<Cloud*>;
  this->list_99p_30m_xd = new list<Cloud*>;

  this->list_10p_40m_xd = new list<Cloud*>;
  this->list_25p_40m_xd = new list<Cloud*>;
  this->list_50p_40m_xd = new list<Cloud*>;
  this->list_99p_40m_xd = new list<Cloud*>;

  this->list_10p_xm = new list<Cloud*>;
  this->list_25p_xm = new list<Cloud*>;
  this->list_50p_xm = new list<Cloud*>;
  this->list_99p_xm = new list<Cloud*>;

  this->list_Spec_dist = new list<Cloud*>;
  this->list_Spec_angle = new list<Cloud*>;

  this->list_Reference = new list<Cloud*>;
  this->list_Ref_dist = new list<Cloud*>;
  this->list_Ref_angle = new list<Cloud*>;

  this->listsCompiled = false;
  this->ref_Spectralon = false;
  this->ref_Sphere = false;

  this->path_calibrationTargets = "../../media/calibrationTargets";
  this->path_sphereTarget = path_calibrationTargets + "/Sphere";
  this->path_sphereTarget_80d = path_calibrationTargets + "/Sphere_80d";
  this->path_sphereTarget_add80d = path_calibrationTargets + "/Sphere_additional_80d";
  this->path_spectralonTarget_dist = path_calibrationTargets + "/Spectralon_distance";
  this->path_spectralonTarget_angle = path_calibrationTargets + "/Spectralon_angle/10m";

  //---------------------------
}
Reference::~Reference(){}

//Load functions
bool Reference::load_SphereTarget_precomp(){
  list<Cloud*>* list_ref = new list<Cloud*>;
  if(isref_Sphere()) return false;
  //---------------------------

  //Load sphere targets
  this->load_calibrationTargets(list_ref, path_sphereTarget);

  //Inserte into sphere list
  for(int i=0; i<list_ref->size() ;i++){
    Cloud* cloud = *next(list_ref->begin(),i);

    attribManager->compute_attribut_cloud(cloud);
    list_Sphere->push_back(cloud);
    list_Reference->push_back(cloud);
    ref_Sphere = true;
  }

  //---------------------------
  return true;
}
bool Reference::load_SphereTarget_precomp_80d(){
  list<Cloud*>* list_ref = new list<Cloud*>;
  if(isref_Sphere()) return false;
  //---------------------------

  //Load sphere targets
  this->load_calibrationTargets(list_ref, path_sphereTarget_80d);

  //Insert into sphere list
  for(int i=0; i<list_ref->size() ;i++){
    Cloud* cloud = *next(list_ref->begin(),i);

    attribManager->compute_attribut_cloud(cloud);
    list_Sphere->push_back(cloud);
    list_Reference->push_back(cloud);
    ref_Sphere = true;
  }

  //---------------------------
  return true;
}
bool Reference::load_SphereTarget_precomp_add80d(){
  list<Cloud*>* list_ref = new list<Cloud*>;
  if(isref_Sphere()) return false;
  //---------------------------

  //Load sphere targets
  bool sucess = load_calibrationTargets(list_ref, path_sphereTarget_add80d);
  if(sucess == false){
    cout<<"Problem loading list of reference targets"<<endl;
  }

  //Inserte into sphere list
  for(int i=0; i<list_ref->size() ;i++){
    Cloud* cloud = *next(list_ref->begin(),i);

    attribManager->compute_attribut_cloud(cloud);
    list_Sphere->push_back(cloud);
    list_Reference->push_back(cloud);
    ref_Sphere = true;
  }

  //---------------------------
  return true;
}
bool Reference::load_SpectralonTarget(){
  char path[PATH_MAX];
  list<Cloud*>* list_ref = new list<Cloud*>;
  //---------------------------

  this->load_calibrationTargets(list_ref, path_spectralonTarget_dist);
  this->load_calibrationTargets(list_ref, path_spectralonTarget_angle);
  this->compute_list(list_ref);

  //---------------------------
  return true;
}
bool Reference::load_calibrationTargets(list<Cloud*>* list, string path_str){
  /*char path[PATH_MAX];
  vector<string> list_path;
  //---------------------------

  if(realpath(path_str.c_str(), path)){
    //Get list of file in folder
    for(const auto& entry : std::experimental::filesystem::directory_iterator(path)){
      string name = entry.path();
      list_path.push_back(name);
    }

    //Sort the list
    sort(list_path.begin(), list_path.end());

    //Load all files
    for(int i=0; i<list_path.size(); i++){
      loaderManager->load_cloud_silent(list_path[i]);
      Cloud* cloud = loaderManager->get_createdcloud();
      list->push_back(cloud);
    }
  }

  //---------------------------
  if(list->size() == 0){
    return false;
  }*/
  return true;
}
bool Reference::load_References_path(string path){
  list<Cloud*>* list_ref = new list<Cloud*>;
  //---------------------------

  this->load_calibrationTargets(list_ref, path);
  this->compute_list(list_ref);

  //---------------------------
  return true;
}

//Reference lists
bool Reference::clear(){
  //---------------------------

  this->listsCompiled = false;
  this->ref_Spectralon = false;
  this->ref_Sphere = false;

  list_Sphere->clear();
  list_Spectralon->clear();
  list_Reference->clear();

  list_Ref_dist->clear();
  list_Ref_angle->clear();

  list_10p_xm_xd->clear();
  list_25p_xm_xd->clear();
  list_50p_xm_xd->clear();
  list_99p_xm_xd->clear();
  list_xp_10m_xd->clear();
  list_xp_20m_xd->clear();
  list_xp_30m_xd->clear();
  list_xp_40m_xd->clear();

  list_10p_05m_xd->clear();
  list_25p_05m_xd->clear();
  list_50p_05m_xd->clear();
  list_99p_05m_xd->clear();
  list_10p_10m_xd->clear();
  list_25p_10m_xd->clear();
  list_50p_10m_xd->clear();
  list_99p_10m_xd->clear();
  list_10p_20m_xd->clear();
  list_25p_20m_xd->clear();
  list_50p_20m_xd->clear();
  list_99p_20m_xd->clear();
  list_10p_30m_xd->clear();
  list_25p_30m_xd->clear();
  list_50p_30m_xd->clear();
  list_99p_30m_xd->clear();
  list_10p_40m_xd->clear();
  list_25p_40m_xd->clear();
  list_50p_40m_xd->clear();
  list_99p_40m_xd->clear();

  list_10p_xm->clear();
  list_25p_xm->clear();
  list_50p_xm->clear();
  list_99p_xm->clear();

  list_Spec_angle->clear();
  list_Spec_dist->clear();

  //---------------------------
  return true;
}
bool Reference::compute_list(list<Cloud*>* list){
  /*bool sucess = true;
  this->clear();
  //---------------------------

  //General lists
  for(int i=0; i<list->size() ;i++){
    Cloud* cloud = *next(list->begin(),i);

    if(cloud->name.find("Sphere") != std::string::npos){
      attribManager->compute_attribut_cloud(cloud);
      list_Sphere->push_back(cloud);
      list_Reference->push_back(cloud);
      ref_Sphere = true;
    }
    if(cloud->name.find("Spectralon") != std::string::npos){
      attribManager->compute_attribut_cloud(cloud);
      list_Spectralon->push_back(cloud);
      list_Reference->push_back(cloud);
      ref_Spectralon = true;
    }
  }

  //Specialized lists
  if(ref_Sphere){
    sucess = this->extract_listSphere();
  }
  if(ref_Spectralon){
    sucess = this->extract_listSpectralon();
  }

  //---------------------------
  if(list_Reference->size() != 0 && sucess){
    listsCompiled = true;
    return true;
  }else{
    return false;
  }*/
  return true;
}
bool Reference::extract_listSphere(){
  //---------------------------

  for(int i=0;i<list_Sphere->size();i++){
    Cloud* cloud = *next(list_Sphere->begin(),i);

    list_Ref_dist->push_back(cloud);
    list_Ref_angle->push_back(cloud);
  }

  //--------------------
  return true;
}
bool Reference::extract_listSpectralon(){
  /*//---------------------------

  for(int i=0;i<list_Spectralon->size();i++){
    Cloud* cloud = *next(list_Spectralon->begin(),i);

    //xp
    if(cloud->name.find("10p") != std::string::npos){
      list_10p_xm_xd->push_back(cloud);
    }
    if(cloud->name.find("25p") != std::string::npos){
      list_25p_xm_xd->push_back(cloud);
    }
    if(cloud->name.find("50p") != std::string::npos){
      list_50p_xm_xd->push_back(cloud);
    }
    if(cloud->name.find("99p") != std::string::npos){
      list_99p_xm_xd->push_back(cloud);
    }

    //Angles
    //xd by p 05m + xd xm
    if(cloud->name.find("d") != std::string::npos &&
      cloud->name.find("_5m") != std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_05m_xd->push_back(cloud);
      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_05m_xd->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_05m_xd->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_05m_xd->push_back(cloud);
      }
    }
    //xd by p 10m + xd xm
    if(cloud->name.find("0d") != std::string::npos &&
      cloud->name.find("10m") != std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_10m_xd->push_back(cloud);

      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_10m_xd->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_10m_xd->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_10m_xd->push_back(cloud);
      }
    }

    //xd by p 20m + xd xm
    if(cloud->name.find("0d") != std::string::npos &&
      cloud->name.find("20m") != std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_20m_xd->push_back(cloud);
      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_20m_xd->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_20m_xd->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_20m_xd->push_back(cloud);
      }
    }
    //xd by p 30m + xd xm
    if(cloud->name.find("0d") != std::string::npos &&
      cloud->name.find("30m") != std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_30m_xd->push_back(cloud);
      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_30m_xd->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_30m_xd->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_30m_xd->push_back(cloud);
      }
    }
    //xd by p 40m + xd xm
    if(cloud->name.find("0d") != std::string::npos &&
      cloud->name.find("40m") != std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_40m_xd->push_back(cloud);
      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_40m_xd->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_40m_xd->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_40m_xd->push_back(cloud);
      }
    }

    if(cloud->name.find("0d") != std::string::npos){
      //List Spectralon angle by distance
      if(cloud->name.find("10m") != std::string::npos){
        list_xp_10m_xd->push_back(cloud);
      }
      if(cloud->name.find("20m") != std::string::npos){
        list_xp_20m_xd->push_back(cloud);
      }
      if(cloud->name.find("30m") != std::string::npos){
        list_xp_30m_xd->push_back(cloud);
      }
      if(cloud->name.find("40m") != std::string::npos){
        list_xp_40m_xd->push_back(cloud);
      }
    }

    //all m by p
    if(cloud->name.find("d") == std::string::npos){
      if(cloud->name.find("10p") != std::string::npos){
        list_10p_xm->push_back(cloud);
      }
      if(cloud->name.find("25p") != std::string::npos){
        list_25p_xm->push_back(cloud);
      }
      if(cloud->name.find("50p") != std::string::npos){
        list_50p_xm->push_back(cloud);
      }
      if(cloud->name.find("99p") != std::string::npos){
        list_99p_xm->push_back(cloud);
      }
    }

    //all d 99p 10m
    if(cloud->name.find("0d") != std::string::npos &&
      cloud->name.find("10m") != std::string::npos &&
      cloud->name.find("99p") != std::string::npos){
      list_Ref_angle->push_back(cloud);
    }
    //all d
    if(cloud->name.find("0d") != std::string::npos){
      list_Spec_angle->push_back(cloud);
    }
    //all d
    if(cloud->name.find("0d") == std::string::npos){
      list_Spec_dist->push_back(cloud);
    }
    //99p xm
    if(cloud->name.find("0d") == std::string::npos &&
      cloud->name.find("99p") != std::string::npos){
      list_Ref_dist->push_back(cloud);
    }
  }

  //--------------------*/
  return true;
}

//Subfunctions
bool Reference::is_cloudReference(Cloud* cloud){
  /*//---------------------------

  for(int i=0; i<list_Sphere->size() ;i++){
    Cloud* cloud_list = *next(list_Sphere->begin(),i);
    if(cloud->name == cloud_list->Name){
      return true;
    }
  }

  for(int i=0; i<list_Spectralon->size() ;i++){
    Cloud* cloud_list = *next(list_Spectralon->begin(),i);
    if(cloud->name == cloud_list->Name){
      return true;
    }
  }

  //---------------------------*/
  return false;
}

Cloud* Reference::get_specificSpectralon(string p, string m, bool degree){
  Cloud* cloud_out;
  //---------------------------

  /*for(int i=0;i<list_Spectralon->size();i++){
    Cloud* cloud = *next(list_Spectralon->begin(),i);

    if(cloud->name.find(p) == std::string::npos &&
      cloud->name.find(m) == std::string::npos){

      if(degree){
        if(cloud->name.find("0d") != std::string::npos){
          cloud_out = cloud;
        }
      }else{
        if(cloud->name.find("0d") == std::string::npos){
          cloud_out = cloud;
        }
      }
    }
  }*/

  //---------------------------
  return cloud_out;
}
Cloud* Reference::get_specificSphere(string m){
  Cloud* cloud_out;
  //---------------------------

  /*for(int i=0;i<list_Sphere->size();i++){
    Cloud* cloud = *next(list_Sphere->begin(),i);

    if(cloud->name.find(m) != std::string::npos){
      cloud_out = cloud;
    }
  }*/

  //---------------------------
  return cloud_out;
}
