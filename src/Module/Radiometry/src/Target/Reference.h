#if !defined(REFERENCE_H)
#define REFERENCE_H

#include "../../../../common.h"

class Node_engine;
class Attribut;
class Loader;


class Reference
{
public:
  //Constructor / Destructor
  Reference(Node_engine* node);
  ~Reference();

public:
  //Load functions
  bool load_SphereTarget_precomp();
  bool load_SphereTarget_precomp_80d();
  bool load_SphereTarget_precomp_add80d();
  bool load_SpectralonTarget();
  bool load_References_path(string path);

  //Reference lists
  bool clear();
  bool compute_list(list<Collection*>* list);
  bool extract_listSphere();
  bool extract_listSpectralon();
  bool extract_listTest();

  //Subfunctions
  bool is_cloudReference(Collection* collection);
  bool load_calibrationTargets(list<Collection*>* list, string path_str);
  Collection* get_specificSpectralon(string p, string m, bool degree);
  Collection* get_specificSphere(string m);

  inline list<Collection*>* get_listSphere(){return list_Sphere;}
  inline list<Collection*>* get_listSpectralon(){return list_Spectralon;}

  //Angles
  //==============================================================
  inline list<Collection*>* get_listSpecAngle(){return list_Spec_angle;}
  inline list<Collection*>* get_listAngle(){return list_Ref_angle;}

  inline list<Collection*>* get_list_10p_xm_xd(){return list_10p_xm_xd;}
  inline list<Collection*>* get_list_25p_xm_xd(){return list_25p_xm_xd;}
  inline list<Collection*>* get_list_50p_xm_xd(){return list_50p_xm_xd;}
  inline list<Collection*>* get_list_99p_xm_xd(){return list_99p_xm_xd;}

  inline list<Collection*>* get_list_10p_05m_xd(){return list_10p_05m_xd;}
  inline list<Collection*>* get_list_25p_05m_xd(){return list_25p_05m_xd;}
  inline list<Collection*>* get_list_50p_05m_xd(){return list_50p_05m_xd;}
  inline list<Collection*>* get_list_99p_05m_xd(){return list_99p_05m_xd;}

  inline list<Collection*>* get_list_10p_10m_xd(){return list_10p_10m_xd;}
  inline list<Collection*>* get_list_25p_10m_xd(){return list_25p_10m_xd;}
  inline list<Collection*>* get_list_50p_10m_xd(){return list_50p_10m_xd;}
  inline list<Collection*>* get_list_99p_10m_xd(){return list_99p_10m_xd;}

  inline list<Collection*>* get_list_10p_20m_xd(){return list_10p_20m_xd;}
  inline list<Collection*>* get_list_25p_20m_xd(){return list_25p_20m_xd;}
  inline list<Collection*>* get_list_50p_20m_xd(){return list_50p_20m_xd;}
  inline list<Collection*>* get_list_99p_20m_xd(){return list_99p_20m_xd;}

  inline list<Collection*>* get_list_10p_30m_xd(){return list_10p_30m_xd;}
  inline list<Collection*>* get_list_25p_30m_xd(){return list_25p_30m_xd;}
  inline list<Collection*>* get_list_50p_30m_xd(){return list_50p_30m_xd;}
  inline list<Collection*>* get_list_99p_30m_xd(){return list_99p_30m_xd;}

  inline list<Collection*>* get_list_10p_40m_xd(){return list_10p_40m_xd;}
  inline list<Collection*>* get_list_25p_40m_xd(){return list_25p_40m_xd;}
  inline list<Collection*>* get_list_50p_40m_xd(){return list_50p_40m_xd;}
  inline list<Collection*>* get_list_99p_40m_xd(){return list_99p_40m_xd;}

  inline list<Collection*>* get_list_xp_10m_xd(){return list_xp_10m_xd;}
  inline list<Collection*>* get_list_xp_20m_xd(){return list_xp_20m_xd;}
  inline list<Collection*>* get_list_xp_30m_xd(){return list_xp_30m_xd;}
  inline list<Collection*>* get_list_xp_40m_xd(){return list_xp_40m_xd;}
  //==============================================================

  //Distances
  //==============================================================
  inline list<Collection*>* get_listSpecDist(){return list_Spec_dist;}
  inline list<Collection*>* get_listDist(){return list_Ref_dist;}

  inline list<Collection*>* get_list_10p_xm(){return list_10p_xm;}
  inline list<Collection*>* get_list_25p_xm(){return list_25p_xm;}
  inline list<Collection*>* get_list_50p_xm(){return list_50p_xm;}
  inline list<Collection*>* get_list_99p_xm(){return list_99p_xm;}
  //==============================================================

  inline void set_listsCompiled(bool value){this->listsCompiled = value;}
  inline list<Collection*>* get_listReference(){return list_Reference;}

  inline bool isref_Spectralon(){return ref_Spectralon;}
  inline bool isref_Sphere(){return ref_Sphere;}
  inline bool is_listsCompiled(){return listsCompiled;}
  inline string* get_path_calibrationTargets(){return &path_calibrationTargets;}
  inline string* get_path_sphereTarget_add80d(){return &path_sphereTarget_add80d;}
  inline string* get_path_spectralonTarget_dist(){return &path_spectralonTarget_dist;}
  inline string* get_path_spectralonTarget_angle(){return &path_spectralonTarget_angle;}

private:
  Attribut* attribManager;
  Loader* loaderManager;

  list<Collection*>* list_Sphere;
  list<Collection*>* list_Spectralon;

  list<Collection*>* list_10p_xm_xd;
  list<Collection*>* list_25p_xm_xd;
  list<Collection*>* list_50p_xm_xd;
  list<Collection*>* list_99p_xm_xd;
  list<Collection*>* list_xp_10m_xd;
  list<Collection*>* list_xp_20m_xd;
  list<Collection*>* list_xp_30m_xd;
  list<Collection*>* list_xp_40m_xd;
  list<Collection*>* list_Spec_dist;
  list<Collection*>* list_Spec_angle;

  //Angles
  //===========================
  list<Collection*>* list_10p_05m_xd;
  list<Collection*>* list_25p_05m_xd;
  list<Collection*>* list_50p_05m_xd;
  list<Collection*>* list_99p_05m_xd;

  list<Collection*>* list_10p_10m_xd;
  list<Collection*>* list_25p_10m_xd;
  list<Collection*>* list_50p_10m_xd;
  list<Collection*>* list_99p_10m_xd;

  list<Collection*>* list_10p_20m_xd;
  list<Collection*>* list_25p_20m_xd;
  list<Collection*>* list_50p_20m_xd;
  list<Collection*>* list_99p_20m_xd;

  list<Collection*>* list_10p_30m_xd;
  list<Collection*>* list_25p_30m_xd;
  list<Collection*>* list_50p_30m_xd;
  list<Collection*>* list_99p_30m_xd;

  list<Collection*>* list_10p_40m_xd;
  list<Collection*>* list_25p_40m_xd;
  list<Collection*>* list_50p_40m_xd;
  list<Collection*>* list_99p_40m_xd;
  //===========================

  list<Collection*>* list_10p_xm;
  list<Collection*>* list_25p_xm;
  list<Collection*>* list_50p_xm;
  list<Collection*>* list_99p_xm;

  list<Collection*>* list_Reference;
  list<Collection*>* list_Ref_dist;
  list<Collection*>* list_Ref_angle;

  string path_calibrationTargets;
  string path_sphereTarget;
  string path_sphereTarget_80d;
  string path_sphereTarget_add80d;
  string path_spectralonTarget_dist;
  string path_spectralonTarget_angle;

  bool listsCompiled;
  bool ref_Spectralon, ref_Sphere;
};

#endif
