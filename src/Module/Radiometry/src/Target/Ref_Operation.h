#if !defined(REF_OPERATION_H)
#define REF_OPERATION_H

#include "../../../../common.h"

class BundleByClass;
class PolyRegression;
class Reference;


class Ref_Operation
{
public:
  //Constructor / Destructor
  Ref_Operation(Reference* refManager);
  ~Ref_Operation();

public:
  //Subfunctions
  void compute_sortByIt(vector<float>& vec1, vector<float>& vec2, vector<float>& vec3);
  vector<vec3> get_sphereData(int i);

  //Space parameter
  bool compute_ParameterSpace_Sphere(float bundle);
  bool compute_ParameterSpace_Spectralon();
  inline vector<vec3> get_ParameterSpace(){return PS;}

  //Surfacic regression approach
  bool compute_Surfacic_gloabPiecewise(float R_1, float R_2);
  inline vector<vector<float>> get_Surfacique_cIt_b(){return Pfeifer_cIt_b;}
  inline vector<vector<float>> get_Surfacique_I01_b(){return Pfeifer_I01_b;}
  inline vector<vector<float>> get_Surfacique_R_b(){return Pfeifer_R_b;}
  inline vector<vector<float>> get_Surfacique_cIt_m(){return Pfeifer_cIt_m;}
  inline vector<vector<float>> get_Surfacique_I01_m(){return Pfeifer_I01_m;}
  inline vector<vector<float>> get_Surfacique_R_m(){return Pfeifer_R_m;}
  inline vector<vector<float>> get_Surfacique_cIt_e(){return Pfeifer_cIt_e;}
  inline vector<vector<float>> get_Surfacique_I01_e(){return Pfeifer_I01_e;}
  inline vector<vector<float>> get_Surfacique_R_e(){return Pfeifer_R_e;}

  //Sphere IbyR
  bool compute_Sphere_IbyR();
  inline vector<float> get_Sphere_IbyR_R(){return Sphere_IbyR_R;}
  inline vector<float> get_Sphere_IbyR_I01(){return Sphere_IbyR_I01;}
  inline vector<float> get_Sphere_IbyR_I01_std(){return Sphere_IbyR_I01_std;}

  //Sphere IbycIt
  bool compute_Sphere_IbycIt();
  inline vector<vector<float>> get_Sphere_IbycIt_std(){return Sphere_IbycIt_std;}
  inline vector<vector<float>> get_Sphere_IbycIt_cIt(){return Sphere_IbycIt_cIt;}
  inline vector<vector<float>> get_Sphere_IbycIt_It(){return Sphere_IbycIt_It;}
  inline vector<vector<float>> get_Sphere_IbycIt_I01(){return Sphere_IbycIt_I;}
  inline vector<vector<float>> get_Sphere_IbycIt_R(){return Sphere_IbycIt_R;}

  //Spectralon IbyR
  bool compute_Spectralon_IbyR();
  inline vector<float> get_Spec_IbyR_R_99(){return Spec_IbyR_R_99;}
  inline vector<float> get_Spec_IbyR_R_50(){return Spec_IbyR_R_50;}
  inline vector<float> get_Spec_IbyR_R_25(){return Spec_IbyR_R_25;}
  inline vector<float> get_Spec_IbyR_R_10(){return Spec_IbyR_R_10;}

  inline vector<float> get_Spec_IbyR_I01_99(){return Spec_IbyR_I01_99;}
  inline vector<float> get_Spec_IbyR_I01_50(){return Spec_IbyR_I01_50;}
  inline vector<float> get_Spec_IbyR_I01_25(){return Spec_IbyR_I01_25;}
  inline vector<float> get_Spec_IbyR_I01_10(){return Spec_IbyR_I01_10;}

  inline vector<float> get_Spec_IbyR_I2048_99(){return Spec_IbyR_I2048_99;}
  inline vector<float> get_Spec_IbyR_I2048_50(){return Spec_IbyR_I2048_50;}
  inline vector<float> get_Spec_IbyR_I2048_25(){return Spec_IbyR_I2048_25;}
  inline vector<float> get_Spec_IbyR_I2048_10(){return Spec_IbyR_I2048_10;}

  inline vector<float> get_Spec_IbyR_std_99(){return Spec_IbyR_std_99;}
  inline vector<float> get_Spec_IbyR_std_50(){return Spec_IbyR_std_50;}
  inline vector<float> get_Spec_IbyR_std_25(){return Spec_IbyR_std_25;}
  inline vector<float> get_Spec_IbyR_std_10(){return Spec_IbyR_std_10;}

  inline vector<float> get_Spec_IbyR_It_99(){return Spec_IbyR_It_99;}
  inline vector<float> get_Spec_IbyR_It_50(){return Spec_IbyR_It_50;}
  inline vector<float> get_Spec_IbyR_It_25(){return Spec_IbyR_It_25;}
  inline vector<float> get_Spec_IbyR_It_10(){return Spec_IbyR_It_10;}

  inline vector<float> get_Spec_IbyR_nb_99(){return Spec_IbyR_nb_99;}
  inline vector<float> get_Spec_IbyR_nb_50(){return Spec_IbyR_nb_50;}
  inline vector<float> get_Spec_IbyR_nb_25(){return Spec_IbyR_nb_25;}
  inline vector<float> get_Spec_IbyR_nb_10(){return Spec_IbyR_nb_10;}

  //Spectralon IbycIt
  bool compute_Spectralon_IbycIt(int distance);
  inline vector<float> get_Spec_IbycIt_I01_99(){return Spec_IbycIt_I01_99;}
  inline vector<float> get_Spec_IbycIt_I01_50(){return Spec_IbycIt_I01_50;}
  inline vector<float> get_Spec_IbycIt_I01_25(){return Spec_IbycIt_I01_25;}
  inline vector<float> get_Spec_IbycIt_I01_10(){return Spec_IbycIt_I01_10;}

  inline vector<float> get_Spec_IbycIt_cIt_99(){return Spec_IbycIt_cIt_99;}
  inline vector<float> get_Spec_IbycIt_cIt_50(){return Spec_IbycIt_cIt_50;}
  inline vector<float> get_Spec_IbycIt_cIt_25(){return Spec_IbycIt_cIt_25;}
  inline vector<float> get_Spec_IbycIt_cIt_10(){return Spec_IbycIt_cIt_10;}

  inline vector<float> get_Spec_IbycIt_It_99(){return Spec_IbycIt_It_99;}
  inline vector<float> get_Spec_IbycIt_It_50(){return Spec_IbycIt_It_50;}
  inline vector<float> get_Spec_IbycIt_It_25(){return Spec_IbycIt_It_25;}
  inline vector<float> get_Spec_IbycIt_It_10(){return Spec_IbycIt_It_10;}

  inline vector<float> get_Spec_IbycIt_std_99(){return Spec_IbycIt_std_99;}
  inline vector<float> get_Spec_IbycIt_std_50(){return Spec_IbycIt_std_50;}
  inline vector<float> get_Spec_IbycIt_std_25(){return Spec_IbycIt_std_25;}
  inline vector<float> get_Spec_IbycIt_std_10(){return Spec_IbycIt_std_10;}

  inline vector<float> get_Spec_IbycIt_R_99(){return Spec_IbycIt_R_99;}
  inline vector<float> get_Spec_IbycIt_R_50(){return Spec_IbycIt_R_50;}
  inline vector<float> get_Spec_IbycIt_R_25(){return Spec_IbycIt_R_25;}
  inline vector<float> get_Spec_IbycIt_R_10(){return Spec_IbycIt_R_10;}

  inline vector<float> get_Spec_IbycIt_nb_99(){return Spec_IbycIt_nb_99;}
  inline vector<float> get_Spec_IbycIt_nb_50(){return Spec_IbycIt_nb_50;}
  inline vector<float> get_Spec_IbycIt_nb_25(){return Spec_IbycIt_nb_25;}
  inline vector<float> get_Spec_IbycIt_nb_10(){return Spec_IbycIt_nb_10;}

  inline Reference* get_refManager(){return refManager;}

private:
  BundleByClass* bundler;
  Reference* refManager;

  //Parameter space
  vector<vec3> PS;

  //Pfeifer
  vector<vector<float>> Pfeifer_cIt_b;
  vector<vector<float>> Pfeifer_I01_b;
  vector<vector<float>> Pfeifer_R_b;
  vector<vector<float>> Pfeifer_cIt_m;
  vector<vector<float>> Pfeifer_I01_m;
  vector<vector<float>> Pfeifer_R_m;
  vector<vector<float>> Pfeifer_cIt_e;
  vector<vector<float>> Pfeifer_I01_e;
  vector<vector<float>> Pfeifer_R_e;

  //Sphere IbyR
  vector<float> Sphere_IbyR_I01;
  vector<float> Sphere_IbyR_R;
  vector<float> Sphere_IbyR_I01_std;

  //Sphere IbycIt
  vector<vector<float>> Sphere_IbycIt_I;
  vector<vector<float>> Sphere_IbycIt_std;
  vector<vector<float>> Sphere_IbycIt_cIt;
  vector<vector<float>> Sphere_IbycIt_It;
  vector<vector<float>> Sphere_IbycIt_R;

  //Spectralon IbyR
  vector<float> Spec_IbyR_R_99;
  vector<float> Spec_IbyR_R_50;
  vector<float> Spec_IbyR_R_25;
  vector<float> Spec_IbyR_R_10;

  vector<float> Spec_IbyR_I01_99;
  vector<float> Spec_IbyR_I01_50;
  vector<float> Spec_IbyR_I01_25;
  vector<float> Spec_IbyR_I01_10;

  vector<float> Spec_IbyR_I2048_99;
  vector<float> Spec_IbyR_I2048_50;
  vector<float> Spec_IbyR_I2048_25;
  vector<float> Spec_IbyR_I2048_10;

  vector<float> Spec_IbyR_std_99;
  vector<float> Spec_IbyR_std_50;
  vector<float> Spec_IbyR_std_25;
  vector<float> Spec_IbyR_std_10;

  vector<float> Spec_IbyR_It_99;
  vector<float> Spec_IbyR_It_50;
  vector<float> Spec_IbyR_It_25;
  vector<float> Spec_IbyR_It_10;

  vector<float> Spec_IbyR_nb_99;
  vector<float> Spec_IbyR_nb_50;
  vector<float> Spec_IbyR_nb_25;
  vector<float> Spec_IbyR_nb_10;

  //Spectralon IbycIt
  vector<float> Spec_IbycIt_I01_99;
  vector<float> Spec_IbycIt_I01_50;
  vector<float> Spec_IbycIt_I01_25;
  vector<float> Spec_IbycIt_I01_10;

  vector<float> Spec_IbycIt_cIt_99;
  vector<float> Spec_IbycIt_cIt_50;
  vector<float> Spec_IbycIt_cIt_25;
  vector<float> Spec_IbycIt_cIt_10;

  vector<float> Spec_IbycIt_It_99;
  vector<float> Spec_IbycIt_It_50;
  vector<float> Spec_IbycIt_It_25;
  vector<float> Spec_IbycIt_It_10;

  vector<float> Spec_IbycIt_std_99;
  vector<float> Spec_IbycIt_std_50;
  vector<float> Spec_IbycIt_std_25;
  vector<float> Spec_IbycIt_std_10;

  vector<float> Spec_IbycIt_R_99;
  vector<float> Spec_IbycIt_R_50;
  vector<float> Spec_IbycIt_R_25;
  vector<float> Spec_IbycIt_R_10;

  vector<float> Spec_IbycIt_nb_99;
  vector<float> Spec_IbycIt_nb_50;
  vector<float> Spec_IbycIt_nb_25;
  vector<float> Spec_IbycIt_nb_10;
};

#endif
