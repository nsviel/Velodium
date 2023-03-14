#include "Attribut.h"

#include "../Node_operation.h"
#include "../Optimization/Fitting.h"

#include "../../Engine/GPU/GPU_data.h"
#include "../../Scene/Data/Scene.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/Function/fct_terminal.h"

#include "NormalHough/Normals.h"


//Constructor / destructor
Attribut::Attribut(){
  //---------------------------

  this->sceneManager = new Scene();
  this->fitManager = new Fitting();
  this->gpuManager = new GPU_data();

  this->sphereRadius = 0.0695;

  //---------------------------
}
Attribut::~Attribut(){
  //---------------------------

  delete sceneManager;
  delete fitManager;

  //---------------------------
}

//General
void Attribut::compute_attribut_all(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  //---------------------------

  for(int i=0;i<list_collection->size();i++){
    Collection* collection = *next(list_collection->begin(),i);
    Cloud* cloud = (Cloud*)collection->selected_obj;
    this->compute_attribut_subset(cloud);
  }

  //---------------------------
}
void Attribut::compute_attribut_list(list<Collection*>* list){
  //---------------------------

  for(int i=0;i<list->size();i++){
    Collection* collection = *next(list->begin(),i);
    Cloud* cloud = (Cloud*)collection->selected_obj;
    this->compute_attribut_subset(cloud);
  }

  //---------------------------
}
void Attribut::compute_attribut_subset(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<float>& Is = cloud->I;
  vector<float>& dist = cloud->R;
  vector<float>& cosIt = cloud->cosIt;
  vector<vec3>& N = cloud->Nxyz;
  //---------------------------

  //Distances
  this->compute_Distances(cloud);

  //Normal
  if(N.size() == 0){
    this->compute_normals(cloud);
  }

  //Incidence angle
  if(N.size() != 0){
    this->compute_cosIt(cloud);
  }

  //Checking integrity
  this->compute_checkForNan(cloud);

  //---------------------------
  if(Is.size() != XYZ.size()) cout<<"Is data problem - Is: "<<Is.size()<<" XYZ: "<<XYZ.size()<<endl;
  if(dist.size() != XYZ.size()) cout<<"dist data problem"<<endl;
  if(N.size() != XYZ.size()) cout<<"N data problem"<<endl;
  if(cosIt.size() != XYZ.size()){
    cout<<"cosit data problem - XYZ: "<<XYZ.size()<<" - cosIt: "<<cosIt.size()<<endl;
  }
}
void Attribut::compute_attribut_cloud(Collection* collection){
  //---------------------------

  Cloud* cloud = (Cloud*)collection->selected_obj;
  this->compute_attribut_subset(cloud);

  //---------------------------
}
void Attribut::compute_distToScanner(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<float>& dist = cloud->R;
  vec3& scanpos = cloud->root;
  //---------------------------

  dist.clear();
  for(int i=0; i<XYZ.size(); i++){
    float d = sqrt(pow(XYZ[i].x - scanpos.x,2) + pow(XYZ[i].y - scanpos.y,2) + pow(XYZ[i].z - scanpos.z,2));
    dist.push_back(d);
  }

  //---------------------------
}
void Attribut::compute_Distances(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vec3 root = cloud->root;
  //---------------------------

  //Compute distances
  vector<float> dist(XYZ.size());
  for(int i=0; i<XYZ.size(); i++){
    dist[i] = sqrt(pow(XYZ[i].x - root.x,2) + pow(XYZ[i].y - root.y,2) + pow(XYZ[i].z - root.z,2));
  }

  //---------------------------
}
void Attribut::compute_cosIt(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vector<float>& dist = cloud->R;
  vector<float>& cosIt = cloud->cosIt;
  vector<float>& It = cloud->It;
  vec3 root = cloud->root;
  //---------------------------

  //check data
  cosIt.clear(); It.clear();
  if(dist.size() == 0){
    compute_Distances(cloud);
  }
  if(N.size() == 0){
    //cout<<"No normal for cosIt computation"<<endl;
    return;
  }

  //Compute cosIt
  for(int i=0; i<XYZ.size(); i++){
    float cIt = 0;

    //Compute cosIt
    for(int j=0; j<3; j++){
      cIt = cIt + ( -N[i][j] * ( (XYZ[i][j] - root[j]) / dist[i] ));
    }

    //Check for orientation
    if(cIt < 0){
      cIt = -cIt;
    }
    //Check for computability
    if(cIt >= 1){
      cIt = 0.9999;
    }

    cosIt.push_back(cIt);
    It.push_back( acos(cIt) * 180 / M_PI );
  }

  //---------------------------
}

void Attribut::compute_subset_cosIt(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vector<float>& dist = cloud->R;
  vector<float>& cosIt = cloud->cosIt;
  vector<float>& It = cloud->It;
  vec3 root = cloud->root;
  //---------------------------

  //check data
  cosIt.clear(); It.clear();
  if(dist.size() == 0){
    this->compute_subset_distance(cloud);
  }
  if(N.size() == 0){
    cout<<"No normal for cosIt computation"<<endl;
    return;
  }

  //Compute cosIt
  for(int i=0; i<XYZ.size(); i++){
    float cIt = 0;

    //Compute cosIt
    for(int j=0; j<3; j++){
      cIt = cIt + ( -N[i][j] * ( (XYZ[i][j] - root[j]) / dist[i] ));
    }

    //Check for orientation
    if(cIt < 0){
      cIt = -cIt;
    }
    //Check for computability
    if(cIt >= 1){
      cIt = 0.9999;
    }

    cosIt.push_back(cIt);
    It.push_back( acos(cIt) * 180 / M_PI );
  }

  //---------------------------
}
void Attribut::compute_subset_distance(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vec3 root = cloud->root;
  //---------------------------

  //Compute distances
  vector<float> dist(XYZ.size());
  for(int i=0; i<XYZ.size(); i++){
    dist[i] = sqrt(pow(XYZ[i].x - root.x,2) + pow(XYZ[i].y - root.y,2) + pow(XYZ[i].z - root.z,2));
  }

  //---------------------------
  cloud->R = dist;
}

void Attribut::make_supressPoints(Cloud* cloud, vector<int>& idx){
  if(idx.size() == 0)return;
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vector<vec4>& RGB = cloud->rgb;
  vector<float>& Is = cloud->I;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<vec3> XYZ_b;
  vector<vec4> RGB_b;
  vector<float> Is_b;
  vector<vec3> N_b;
  int cpt = 0;

  for(int i=0; i<XYZ.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      XYZ_b.push_back(XYZ[i]);
      if(RGB.size() != 0) RGB_b.push_back(RGB[i]);
      if(Is.size() != 0) Is_b.push_back(Is[i]);
      if(N.size() != 0) N_b.push_back(N[i]);
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //location
  cloud->xyz = XYZ_b;
  cloud->nb_point = XYZ_b.size();

  //attributs
  if(RGB.size() != 0){
    cloud->rgb = RGB_b;
  }
  if(Is.size() != 0){
    cloud->I = Is_b;
  }
  if(N.size() != 0){
    cloud->Nxyz = N_b;
  }

  if(cloud->R.size() != 0){
    this->compute_Distances(cloud);
  }
  if(cloud->cosIt.size() != 0){
    this->compute_cosIt(cloud);
  }
  idx.clear();

  //---------------------------
  gpuManager->update_buffer_location(cloud);
  gpuManager->update_buffer_color(cloud);
}
void Attribut::make_supressPoints(Object_* object, vector<int>& idx){
  if(idx.size() == 0)return;
  vector<vec3>& XYZ = object->xyz;
  vector<vec3>& N = object->Nxyz;
  vector<vec4>& RGB = object->rgb;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<vec3> XYZ_b;
  vector<vec4> RGB_b;
  vector<float> Is_b;
  vector<vec3> N_b;
  int cpt = 0;

  for(int i=0; i<XYZ.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      XYZ_b.push_back(XYZ[i]);
      if(RGB.size() != 0) RGB_b.push_back(RGB[i]);
      if(N.size() != 0) N_b.push_back(N[i]);
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //location
  object->xyz = XYZ_b;
  object->nb_point = XYZ_b.size();

  //attributs
  if(RGB.size() != 0){
    object->rgb = RGB_b;
  }
  if(N.size() != 0){
    object->Nxyz = N_b;
  }

  idx.clear();

  //---------------------------
  gpuManager->update_buffer_location(object);
  gpuManager->update_buffer_color(object);
}
void Attribut::make_supressPoint(Cloud* cloud, int id){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vector<vec4>& RGB = cloud->rgb;
  vector<float>& Is = cloud->I;
  //---------------------------

  //Recreate vector - Fastest delection method
  vector<vec3> XYZ_b, N_b;
  vector<vec4> RGB_b;
  vector<float> Is_b;

  for(int i=0; i<XYZ.size(); i++){
    if(i != id){
      XYZ_b.push_back(XYZ[i]);
      if(RGB.size() != 0) RGB_b.push_back(RGB[i]);
      if(Is.size() != 0) Is_b.push_back(Is[i]);
      if(N.size() != 0) N_b.push_back(N[i]);
    }
  }

  //location
  cloud->xyz = XYZ_b;
  cloud->nb_point = XYZ_b.size();

  //attributs
  if(RGB.size() != 0){
    cloud->rgb = RGB_b;
  }
  if(Is.size() != 0){
    cloud->I = Is_b;
  }
  if(N.size() != 0){
    cloud->Nxyz = N_b;
  }

  if(cloud->R.size() != 0){
    this->compute_Distances(cloud);
  }
  if(cloud->cosIt.size() != 0){
    this->compute_cosIt(cloud);
  }

  cloud->selected.clear();

  //---------------------------
  gpuManager->update_buffer_location(cloud);
  gpuManager->update_buffer_color(cloud);
}
void Attribut::make_supressPoints(vector<vec3>& vec, vector<int>& idx){
  if(idx.size() == 0)return;
  //---------------------------

  //Sort indice vector
  sort(idx.begin(), idx.end());

  //Recreate vector -> Fastest delection method
  vector<vec3> vec_b;
  int cpt = 0;

  for(int i=0; i<vec.size(); i++){
    //if i different from not taking account point
    if(i != idx[cpt]){
      vec_b.push_back(vec[i]);;
    }
    //if not taking account point, ok, pass to the next
    else{
      cpt++;
    }
  }

  //---------------------------
  idx.clear();
  vec = vec_b;
}
void Attribut::cloudsData(){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  //---------------------------

  ofstream myfile;
  myfile.open ("Clouds_Name-Is_Ic_It_cosIt_R.txt");
  myfile << "\n";
  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);
    Cloud* cloud = (Cloud*)collection->selected_obj;
    Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();

    if(cloud->name.find("rdm") != std::string::npos){
      vector<float>& Is = cloud->I;
      vector<float>& Is_ini = list_obj_init->I;
      vector<float>& It = list_obj_init->It;
      vector<float>& cosIt = list_obj_init->cosIt;
      vector<float>& dist = list_obj_init->R;

      if(cosIt.size() == 0 && cloud->Nxyz.size() != 0) compute_cosIt(cloud);

      myfile << cloud->name<< " ";
      myfile << fct_mean(Is_ini)<<" "<<fct_mean(Is)<<" "<<fct_mean(It)<<" "<<fct_mean(cosIt)<<" "<<fct_mean(dist);
      myfile << "\n";
    }
  }

  //---------------------------
  myfile.close();
  cout<<"Data extracted in Clouds_Name-*.txt"<<endl;
}
vector<float> Attribut::get_z_vector(vector<vec3>& xyz){
  vector<float> z_vec;
  //---------------------------

  for(int i=0; i<xyz.size(); i++){
    z_vec.push_back(xyz[i].z);
  }

  //---------------------------
  return z_vec;
}

//Normal
void Attribut::compute_normals(Cloud* cloud){
  //---------------------------

  if(cloud->name.find("Sphere") != std::string::npos || cloud->name.find("sphere") != std::string::npos){
    this->compute_normals_sphere(cloud);
  }else if(cloud->name.find("Spectralon") != std::string::npos || cloud->name.find("spectralon") != std::string::npos){
    this->compute_normals_planFitting(cloud);
  }else{
    /*#ifdef PCL_FUNCTIONS_H
    pcl_functions pclManager;
    pclManager.compute_normals_PCL(cloud);
    #endif*/
  }

  //---------------------------
}
void Attribut::compute_normals_Hough(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;

  int K = 100;
  int T = 1000;
  int n_phi = 15;
  int n_rot = 5;
  bool ua = false;
  float tol_angle_rad = 0.79;
  int k_density = 5;

  tic();
  //-------------------------

  //Convert to Eigen cloud
  Eigen::MatrixX3d pc = Eigen::MatrixX3d::Zero(XYZ.size(),3);
  Eigen::MatrixX3d normals = Eigen::MatrixX3d::Zero(XYZ.size(),3);
  for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      pc(i,j) = XYZ[i][j];
      normals(i,j) = N[i][j];
    }
  }

  //Compute normals
  Eigen_Normal_Estimator ne(pc,normals);
  ne.set_K(K);
  ne.set_T(T);
  ne.set_density_sensitive(ua);
  ne.set_n_phi(n_phi);
  ne.set_n_rot(n_rot);
  ne.set_tol_angle_rad(tol_angle_rad);
  ne.set_K_density(k_density);
  ne.estimate_normals();

  //Convert to glm cloud
  N.clear();
  for(int i=0; i<XYZ.size(); i++){
    N.push_back(vec3(normals(i,0), normals(i,1), normals(i,2)));
  }

  //---------------------------
  float duration = toc();
  string log = "Normal for " +  cloud->name + " computed " + to_string(duration) + " ms";
  console.AddLog("#", log);
}
void Attribut::compute_normals_sphere(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<float>& dist = cloud->R;
  tic();
  //---------------------------

  //Check data
  if(dist.size() == 0) {compute_Distances(cloud);}

  //Search for nearest point
  float distm, Xm, Ym, Zm;
  float dist_min = fct_min(dist);
  for (int i=0; i<XYZ.size(); i++){
    if(dist[i] == dist_min){
        distm = dist[i];
        Xm = XYZ[i].x;
        Ym = XYZ[i].y;
        Zm = XYZ[i].z;
    }
  }

  //Determine the center of the sphere
  vec3 Center = fitManager->Sphere_FindCenter(cloud);

  //Compute normals
  vector<vec3> N(XYZ.size());
  for (int i=0; i<XYZ.size(); i++){
    for (int j=0; j<3; j++){
      N[i][j] = (XYZ[i][j] - Center[j]) / sphereRadius;
    }
  }
  cloud->Nxyz = N;


  //---------------------------
  float duration = toc();
  string log = "Normal for " +  cloud->name + " computed " + to_string(duration) + " ms";
  console.AddLog("#", log);
}
void Attribut::compute_normals_planXaxis(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vec3 norm, Point;
  N.clear();
  //-------------------------

  vec3 P1 = vec3(10, 0, 10);
  vec3 P2 = vec3(-10, 0, -10);

  //Compute normal to a plane for each points
  for(int i=0; i<XYZ.size(); i++){
    //(glm::cross(c - a, b - a))
    Point = vec3(XYZ[i].x, 0, XYZ[i].z);
    norm = glm::normalize(glm::cross(Point - P1, P2 - P1));
    N.push_back(norm);
  }

  //-------------------------
}
void Attribut::compute_normals_planYaxis(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vec3 norm, Point;
  N.clear();
  //-------------------------

  vec3 P1 = vec3(0, 10, 10);
  vec3 P2 = vec3(0, -10, -10);

  //Compute normal to a plane for each points
  for(int i=0; i<XYZ.size(); i++){
    //(glm::cross(c - a, b - a))
    Point = vec3(0, XYZ[i].y, XYZ[i].z);
    norm = glm::normalize(glm::cross(Point - P1, P2 - P1));
    N.push_back(norm);
  }

  //-------------------------
}
void Attribut::compute_normals_planZaxis(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  vec3 norm, Point;
  N.clear();
  //-------------------------

  vec3 P1 = vec3(100, 100, 0);
  vec3 P2 = vec3(100, -100, 0);

  //Compute normal to a plane for each points
  for(int i=0; i<XYZ.size(); i++){
    //(glm::cross(c - a, b - a))
    Point = vec3(XYZ[i].x, XYZ[i].y, 0);
    norm = glm::normalize(glm::cross(Point - P1, P2 - P1));
    N.push_back(norm);
  }

  //-------------------------
}
void Attribut::compute_normals_planFitting(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  tic();
  //-------------------------

	// copy coordinates to  matrix in Eigen format
	Eigen::Matrix< Vector3f::Scalar, Eigen::Dynamic, Eigen::Dynamic > coord(3, XYZ.size());
	for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      coord(j,i) = XYZ[i][j];
    }
  }

	// calculate centroid
	Vector3f centroid(coord.row(0).mean(), coord.row(1).mean(), coord.row(2).mean());

	// subtract centroid
	coord.row(0).array() -= centroid(0);
  coord.row(1).array() -= centroid(1);
  coord.row(2).array() -= centroid(2);

	// we only need the left-singular matrix here
	auto svd = coord.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
	Vector3f plane_normal = svd.matrixU().rightCols<1>();

  //Store normal data
  N.clear();
  for(int i=0; i<XYZ.size(); i++){
    N.push_back(vec3(plane_normal(0),plane_normal(1),plane_normal(2)));
  }


  //Reoriente normal in the origin direction
  this->compute_normals_reorientToOrigin(cloud);

  //---------------------------
  float duration = toc();
  string log = "Normal for " +  cloud->name + " computed " + to_string(duration) + " ms";
  console.AddLog("#", log);
}
void Attribut::compute_normals_invert(){
  if(!sceneManager->get_is_list_empty()){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
    vector<vec3>& normals = cloud->Nxyz;
    //---------------------------

    for(int i=0; i<normals.size(); i++){
      for(int j=0; j<3; j++){
        normals[i][j] = -normals[i][j];
      }
    }
    list_obj_init->Nxyz = normals;

    //---------------------------
  }
}
void Attribut::compute_normals_reorientToOrigin(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec3>& N = cloud->Nxyz;
  //---------------------------5

  float dist_XYZ, dist_N;
  for(int i=0; i<XYZ.size(); i++){
    dist_XYZ = fct_distance(XYZ[i], vec3(0,0,0));
    dist_N = fct_distance(XYZ[i] + N[i], vec3(0,0,0));

    if(dist_N > dist_XYZ){
      for(int j=0; j<3; j++){
        N[i][j] = -N[i][j];
      }
    }
  }

  //---------------------------
}
void Attribut::compute_checkForNan(Cloud* cloud){
  vector<vec3>& N = cloud->Nxyz;
  vector<float>& cosIt = cloud->cosIt;
  vector<int> idx;
  //---------------------------

  //Compute cosIt
  for(int i=0; i<cosIt.size(); i++){
    if(isnan(N[i].x) == true){
      idx.push_back(i);
    }
    if(isnan(cosIt[i]) == true){
      idx.push_back(i);
    }
  }

  //---------------------------
  this->make_supressPoints(cloud, idx);
}

//Intensity
void Attribut::compute_intensityInversion(){
  if(!sceneManager->get_is_list_empty()){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    //---------------------------

    if(cloud->I.size() != 0){
      vector<float>& Is = cloud->I;

      for(int i=0; i<Is.size(); i++){
        Is[i] = 1 - Is[i];
      }
    }

    //---------------------------
  }
}
void Attribut::compute_colorToIntensity(Cloud* cloud){
  vector<float>& Is_obj = cloud->I;
  vector<vec4>& RGB = cloud->rgb;
  //---------------------------

  //Clear vector
  Is_obj.clear();

  //Convert color into intensity
  if(cloud->has_color){
    for(int i=0; i<RGB.size(); i++){
      float I = (RGB[i].x + RGB[i].y + RGB[i].z) / 3;
      Is_obj.push_back(I);
    }
  }

  //---------------------------
}
void Attribut::compute_intensityToColor(Cloud* cloud){
  vector<float>& Is = cloud->I;
  vector<vec4>& RGB = cloud->rgb;
  //---------------------------

  //Clear vector
  RGB.clear();

  //Convert intensity into color
  for(int i=0; i<Is.size(); i++){
    vec4 new_color = vec4(Is[i], Is[i], Is[i], 1.0f);
    RGB.push_back(new_color);
  }

  //---------------------------
}
void Attribut::fct_convert255to2048(Cloud* cloud){
  vector<float>& Is = cloud->I;
  //-------------------------

  for (int i=0; i<Is.size(); i++){
    Is[i] = (Is[i] * 255 + 2048) / 4096;
  }

  //-------------------------
  //sceneManager->update_subset_IntensityToColor(cloud);
  gpuManager->update_buffer_color(cloud);
}
void Attribut::fct_convert2048to255(Cloud* cloud){
  static bool I_2048 = false;
  vector<float>& Is = cloud->I;
  //-------------------------

  for (int i=0; i<Is.size(); i++){
    Is[i] = (Is[i] * 4096 - 2048) / 255;
  }

  //-------------------------
  //sceneManager->update_subset_IntensityToColor(cloud);
  gpuManager->update_buffer_color(cloud);
}
void Attribut::fct_moins(){
  if(!sceneManager->get_is_list_empty()){
    Collection* collection = sceneManager->get_selected_collection();
    Cloud* cloud = (Cloud*)collection->selected_obj;
    vector<float>& Is = cloud->I;
    vector<vec3>& XYZ = cloud->xyz;
    vector<vec4>& RGB = cloud->rgb;
    vector<float>& cosIt = cloud->cosIt;
    //-------------------------

    cosIt.erase(cosIt.begin());
    cosIt.push_back(0.0f);

    //-------------------------
    //sceneManager->update_subset_IntensityToColor(cloud);
    gpuManager->update_buffer_color(cloud);
  }
}
void Attribut::fct_IsRange(vec2 range){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  vector<float>& Is = cloud->I;
  const vector<float>& Is_ini = list_obj_init->I;
  //---------------------------

  for(int i=0; i<Is.size(); i++){
    if(Is[i] < range.x || Is[i] > range.y){
      Is[i] = -1.0f;
    }else{
      Is[i] = Is_ini[i];
    }
  }

  //-------------------------
  //sceneManager->update_subset_IntensityToColor(cloud);
  gpuManager->update_buffer_color(cloud);
}
vec2 Attribut::get_IsRange(){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  vector<float>& Is = cloud->I;
  //---------------------------

  if(!sceneManager->get_is_list_empty() && Is.size() != 0){
    float min = fct_max(Is);
    for(int i=0; i<Is.size(); i++){
      if(min > Is[i] && Is[i] != -1.0f){
        min = Is[i];
      }
    }

    vec2 range = vec2(min, fct_max(Is));
    return range;
  }else{
    vec2 range = vec2(0, 0);
    return range;
  }

  //---------------------------
}
