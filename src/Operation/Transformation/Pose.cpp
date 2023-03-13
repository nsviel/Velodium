#include "Pose.h"
#include "Transformation.h"

#include "../Optimization/Fitting.h"

#include "../../Specific/Function/fct_math.h"


//Constructor / Destructor
Pose::Pose(){
  //---------------------------

  this->transformManager = new Transformation();
  this->fittingManager = new Fitting();

  this->soilnb_point = 1000;

  //---------------------------
}
Pose::~Pose(){}

// Pose
void Pose::make_centering(Collection* collection){
  vec3& COM = collection->COM;
  //---------------------------

  //Cancel Center of mass from actual position
  this->compute_COM(collection);
  vec3 centering = vec3(-COM.x,-COM.y,-COM.z);
  transformManager->make_translation(collection, centering);

  //Set min(Z) as corresponding to 0
  this->compute_min(collection);
  vec3 redressing = vec3(0, 0, -collection->min[2]);
  transformManager->make_translation(collection, redressing);

  //---------------------------
  console.AddLog("#", "Point cloud centered");
}
void Pose::make_elevation(Collection* collection, float Z){
  Object_* object = collection->selected_obj;
  Object_* object_init = *next(collection->list_obj_init.begin(), collection->ID_obj_selected);
  //---------------------------

  vector<vec3>& XYZ = object->xyz;
  vector<vec3>& XYZ_ini = object_init->xyz;

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].z = XYZ_ini[i].z + Z;
  }

  //---------------------------
}
void Pose::make_positionning(Collection* collection, vec3 pos){
  Object_* object = collection->selected_obj;
  vec3& COM = object->COM;
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = pos.z - COM.z;

  //---------------------------
  transformManager->make_translation(collection, diff);
}
void Pose::make_positionning_XY(Collection* collection, vec3 pos){
  Object_* object = collection->selected_obj;
  vec3& COM = object->COM;
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = 0;

  //---------------------------
  transformManager->make_translation(collection, diff);
}
void Pose::make_positionning_glyph(vector<vec3>& XYZ, vec3& COM, vec3 pos){
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = pos.z - COM.z;

  glm::mat4 Tmat(1.0);
  Tmat[0][3] = diff.x;
  Tmat[1][3] = diff.y;
  Tmat[2][3] = diff.z;

  //Application of the Pose
  for(int i=0;i<XYZ.size();i++){
    //Location
    vec4 XYZ_hom = vec4(XYZ[i], 1.0);
    for(int j=0;j<3;j++){
      XYZ_hom[j] -= COM[j];
    }
    vec4 XYZ_tr = XYZ_hom * Tmat;
    for(int j=0;j<3;j++){
      XYZ_tr[j] += COM[j];
    }
    XYZ[i] = vec3(XYZ_tr.x, XYZ_tr.y, XYZ_tr.z);
  }

  //---------------------------
}
float Pose::make_soilDetermination(Collection* collection){
  Object_* object = collection->selected_obj;
  vector<vec3>& XYZ = object->xyz;
  //---------------------------

  //Retrieve Z coordinates from XYZ
  vector<float> Z_vec;
  for(int i=0; i<XYZ.size(); i++){
    Z_vec.push_back(XYZ[i].z);
  }

  //Sorting Z vector
  sort(Z_vec.begin(), Z_vec.end());

  //Get the XXX first Z values
  vector<float> Z_fist;
  for(int i=0; i<soilnb_point; i++){
    Z_fist.push_back(Z_vec[i]);
  }

  //Compute soil estimation by Z mean;
  float Z_soil = fct_mean(Z_fist);

  //---------------------------
  return Z_soil;
}
void Pose::make_adjustPosToScanner(Collection* collection, float Z_scan){
  Object_* object = collection->selected_obj;
  vector<vec3>& XYZ = object->xyz;
  vec3& min = object->min;
  float Z_soil = make_soilDetermination(collection);
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].z = XYZ[i].z - Z_soil - Z_scan;
  }

  //---------------------------
}

// Orientation
float Pose::make_orientAxis_X(Collection* collection){
  Object_* object = collection->selected_obj;
  //---------------------------

  //Retrieve A & B points
  vector<vec3>& XYZ = object->xyz;
  vec3 A = XYZ[0];
  vec3 B = XYZ[0];
  for(int i=0; i<XYZ.size(); i++){
    if(XYZ[i].x > A.x){
      A = XYZ[i];
    }
    if(XYZ[i].x < B.x){
      B = XYZ[i];
    }
  }

  //Determination of angle with x axis
  float dot = B.x - A.x;
  float det = B.y - A.y;
  float angle = atan2(det, dot);

  //Align with x axis
  vec3 rotation = vec3(0, 0, angle);
  transformManager->make_rotation(collection, object->COM, rotation);

  //---------------------------
  return angle;
}
void Pose::make_alignAxis_X(Collection* collection){
  Object_* object = collection->selected_obj;
  vector<vec3>& XYZ = object->xyz;
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].x = 0;
  }

  //---------------------------
}
float Pose::make_alignAxisX_AB(Collection* collection, vec3 A, vec3 B){
  Object_* object = collection->selected_obj;
  //---------------------------

  //Determination of angle with x axis
  float dot = B.x - A.x;
  float det = B.y - A.y;
  float angle = atan2(det, dot);

  //Align with x axis
  vec3 rotation = vec3(0, 0, angle);
  transformManager->make_rotation(collection, object->COM, rotation);

  //---------------------------
  return angle;
}
float Pose::make_alignAxisY_AB(Collection* collection, vec3 A, vec3 B){
  Object_* object = collection->selected_obj;
  //---------------------------

  //Determination of angle with x axis
  float dot = B.x - A.x;
  float det = -(B.y - A.y);
  float angle = atan2(det, dot);

  //Align with x axis
  vec3 rotation = vec3(0, 0, angle);
  transformManager->make_rotation(collection, object->COM, rotation);

  //---------------------------
  return angle;
}
void Pose::make_alignSelectionToGround(Collection* collection){
  vector<vec3> xyz;
  //---------------------------

  // Get all selected points
  for(int i=0; i<collection->nb_obj; i++){
    Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), i);
    if(cloud->is_visible){
      vector<int>& idx = cloud->selected;
      for(int j=0; j<idx.size(); j++){
        xyz.push_back(cloud->xyz[idx[j]]);
      }
    }
  }

  if(xyz.size() == 0){
    console.AddLog("error" ,"No point selected");
    return;
  }

  //Compute plane fitting on them
  std::pair<vec3, vec3> centroid_normal = fittingManager->plane_fitting_normal(xyz);
  vec3 centroid = centroid_normal.first;
  vec3 normal = centroid_normal.second;
  if(normal.z < 0) normal = -normal;

  // Compute and supress angular angle between normal and basis axis
  float angle = fct_oriented_angle(vec2(normal.x, normal.y), vec2(1, 0));
  mat4 rotation = transformManager->make_rotation(collection, centroid, vec3(0, 0, -angle));
  transformManager->make_Transformation_point(normal, centroid, rotation);

  angle = fct_oriented_angle(vec2(normal.x, normal.z), vec2(0, 1));
  transformManager->make_rotation(collection, centroid, vec3(0, -angle, 0));
  transformManager->make_translation(collection, -centroid);

  //---------------------------
}

// Operation function
mat4 Pose::compute_transformMatrix(vec3 trans, vec3 rotat, vec3 scale){
  glm::mat4 transMat(1.0);
  glm::mat4 Rx(1.0);
  glm::mat4 Ry(1.0);
  glm::mat4 Rz(1.0);
  glm::mat4 Sc(1.0);
  //---------------------------

  Sc[0][0] = scale.x;
  Sc[1][1] = scale.y;
  Sc[2][2] = scale.z;

  Rx[1][1] = cos(rotat.x);
  Rx[2][1] = sin(rotat.x);
  Rx[1][2] = -sin(rotat.x);
  Rx[2][2] = cos(rotat.x);

  Ry[0][0] = cos(rotat.y);
  Ry[0][2] = sin(rotat.y);
  Ry[2][0] = -sin(rotat.y);
  Ry[2][2] = cos(rotat.y);

  Rz[0][0] = cos(rotat.z);
  Rz[1][0] = sin(rotat.z);
  Rz[0][1] = -sin(rotat.z);
  Rz[1][1] = cos(rotat.z);

  transMat = Sc * Rx * Ry * Rz;
  transMat[0][3] = trans.x;
  transMat[1][3] = trans.y;
  transMat[2][3] = trans.z;

  //---------------------------
  return transMat;
}
mat4 Pose::compute_transformMatrix(float tx, float ty, float tz, float rx, float ry, float rz){
  glm::mat4 transMat(1.0);
  glm::mat4 Rx(1.0);
  glm::mat4 Ry(1.0);
  glm::mat4 Rz(1.0);
  //---------------------------

  Rx[1][1]=cos(rx);
  Rx[2][1]=sin(rx);
  Rx[1][2]=-sin(rx);
  Rx[2][2]=cos(rx);

  Ry[0][0]=cos(ry);
  Ry[0][2]=sin(ry);
  Ry[2][0]=-sin(ry);
  Ry[2][2]=cos(ry);

  Rz[0][0]=cos(rz);
  Rz[1][0]=sin(rz);
  Rz[0][1]=-sin(rz);
  Rz[1][1]=cos(rz);

  transMat = Rx * Ry * Rz;
  transMat[0][3] = tx;
  transMat[1][3] = ty;
  transMat[2][3] = tz;

  //---------------------------
  return transMat;
}
vec3 Pose::compute_anglesError(Collection* collection){
  Object_* object = collection->selected_obj;
  mat4 matReal = object->transformation;//.RealPose;
  mat4 matIcp = object->transformation;
  vec3 angleReal = this->compute_anglesFromTransformationMatrix(matReal);
  vec3 angleIcp = this->compute_anglesFromTransformationMatrix(matIcp);
  //---------------------------

  float ex = angleIcp.x - angleReal.x;
  float ey = angleIcp.y - angleReal.y;
  float ez = angleIcp.z - angleReal.z;

  vec3 error = vec3(ex, ey, ez);

  //---------------------------
  return error;
}
vec3 Pose::compute_translationsError(Collection* collection){
  Object_* object = collection->selected_obj;
  mat4 matReal = object->transformation; //.RealPose;
  mat4 matIcp = object->transformation;
  vec3 transReal = this->compute_translFromTransformationMatrix(matReal);
  vec3 transIcp = this->compute_translFromTransformationMatrix(matIcp);
  //---------------------------

  float ex = transIcp.x - transReal.x;
  float ey = transIcp.y - transReal.y;
  float ez = transIcp.z - transReal.z;

  vec3 error = vec3(ex, ey, ez);

  //---------------------------
  return error;
}
vec3 Pose::compute_anglesFromTransformationMatrix(const mat4& mat){
  vec3 angles;
  //---------------------------

  float ax = atan2(mat[2][1], mat[2][2]);
  float ay = atan2(-mat[2][0], sqrt( pow(mat[2][1], 2) + pow(mat[2][2], 2) ) );
  float az = atan2(mat[1][0], mat[0][0]);

  angles = vec3(ax, ay, az);

  //---------------------------
  return angles;
}
vec3 Pose::compute_anglesFromTransformationMatrix(const Eigen::Matrix3f& mat){
  vec3 angles;
  //---------------------------

  float ax = atan2(mat(2,1), mat(2,2));
  float ay = atan2(-mat(2,0), sqrt( pow(mat(2,1), 2) + pow(mat(2,2), 2) ) );
  float az = atan2(mat(1,0), mat(0,0));

  ax = (ax * 180) / M_PI;
  ay = (ay * 180) / M_PI;
  az = (az * 180) / M_PI;

  angles = vec3(ax, ay, az);

  //---------------------------
  return angles;
}
vec3 Pose::compute_anglesFromTransformationMatrix(const Eigen::Matrix3d& mat){
  vec3 angles;
  //---------------------------

  float ax = atan2(mat(2,1), mat(2,2));
  float ay = atan2(-mat(2,0), sqrt( pow(mat(2,1), 2) + pow(mat(2,2), 2) ) );
  float az = atan2(mat(1,0), mat(0,0));

  ax = (ax * 180) / M_PI;
  ay = (ay * 180) / M_PI;
  az = (az * 180) / M_PI;

  angles = vec3(ax, ay, az);

  //---------------------------
  return angles;
}
vec3 Pose::compute_translFromTransformationMatrix(const mat4& mat){
  vec3 translation;
  //---------------------------

  float tx = mat[0][3];
  float ty = mat[1][3];
  float tz = mat[2][3];

  translation = vec3(tx, ty, tz);

  //---------------------------
  return translation;
}
vector<vec3> Pose::compute_transformcloud_XYZ(Collection* collection, mat4 Mat){
  Object_* object = collection->selected_obj;
  vec3& COM = object->COM;
  vector<vec3> XYZ = object->xyz;
  //---------------------------

  //Application of the Pose
  for(int i=0;i<XYZ.size();i++){
    vec4 XYZ_hom = vec4(XYZ[i], 1.0);

    for(int j=0;j<3;j++) XYZ_hom[j] -= COM[j];
    vec4 XYZ_tr = XYZ_hom * Mat;
    for(int j=0;j<3;j++) XYZ_tr[j] += COM[j];

    XYZ[i] = vec3(XYZ_tr.x, XYZ_tr.y, XYZ_tr.z);
  }

  //---------------------------
  return XYZ;
}
void Pose::compute_transformXYZ(vector<vec3>& XYZ, vec3& COM, mat4 Mat){
  //---------------------------

  for(int i=0;i<XYZ.size();i++){
    vec4 XYZ_hom = vec4(XYZ[i], 1.0);

    for(int j=0;j<3;j++){
      XYZ_hom[j] -= COM[j];
    }
    vec4 XYZ_tr = XYZ_hom * Mat;
    for(int j=0;j<3;j++){
      XYZ_tr[j] += COM[j];
    }

    XYZ[i] = vec3(XYZ_tr.x, XYZ_tr.y, XYZ_tr.z);
  }

  //---------------------------
}
void Pose::compute_COM(Collection* collection){
  vec3 COM = vec3(0, 0, 0);
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Object_* object = *next(collection->list_obj.begin(), i);
    COM += fct_centroid(object->xyz);
  }

  //---------------------------
  collection->COM = COM;
}
void Pose::compute_min(Collection* collection){
  vec3 min = vec3(0, 0, 0);
  //---------------------------

  for(int i=0; i<collection->nb_obj; i++){
    Object_* object = *next(collection->list_obj.begin(), i);
    vec3 subset_min = fct_min_vec3(object->xyz);

    for(int j=0; j<3; j++){
      if(subset_min[j] < min[j]){
        min[j] = subset_min[j];
      }
    }
  }

  //---------------------------
  collection->min = min;
}
