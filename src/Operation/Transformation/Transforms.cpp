#include "Transforms.h"

#include "../../Specific/fct_maths.h"


//Constructor / Destructor
Transforms::Transforms(){
  //---------------------------

  this->soilnb_point = 1000;

  //---------------------------
}
Transforms::~Transforms(){}

//Transformation functions
void Transforms::make_translation(Cloud* cloud, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //Apply
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->visibility){
      subset->trans *= translation;
      this->make_Transformation(subset, subset->root, translation);
    }

  }

  //---------------------------
}
void Transforms::make_rotation(Cloud* cloud, vec3 COM, vec3 radian){
  //Rotation matrice creation - rx, ry, rz are in radian !
  glm::mat4 Rx(1.0);
  glm::mat4 Ry(1.0);
  glm::mat4 Rz(1.0);
  //---------------------------

  float rx = radian.x;
  float ry = radian.y;
  float rz = radian.z;

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

  glm::mat4 rotation = Rx * Ry * Rz;

  //Apply
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->visibility){
      subset->rotat *= rotation;
      this->make_Transformation(subset, COM, rotation);
    }

  }

  //---------------------------
}
void Transforms::make_scaling(Cloud* cloud, float Sxyz){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->visibility){
      //Reverso old scaling
      mat4 scaling_reverse(1/subset->scale[0][0]);
      this->make_Transformation_atomic(subset->xyz, subset->COM, scaling_reverse);

      //Scale to new value
      mat4 scaling(Sxyz);
      subset->scale = scaling;
      this->make_Transformation_atomic(subset->xyz, subset->COM, scaling);
    }

  }

  //---------------------------
}

void Transforms::make_translation(Subset* subset, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //---------------------------
  subset->trans *= translation;
  this->make_Transformation(subset, subset->root, translation);
}
void Transforms::make_rotation(Subset* subset, vec3 COM, vec3 radian){
  //Rotation matrice creation - rx, ry, rz are in radian !
  glm::mat4 Rx(1.0);
  glm::mat4 Ry(1.0);
  glm::mat4 Rz(1.0);
  //---------------------------

  float rx = radian.x;
  float ry = radian.y;
  float rz = radian.z;

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

  glm::mat4 rotation = Rx * Ry * Rz;

  //---------------------------
  subset->rotat *= rotation;
  this->make_Transformation(subset, COM, rotation);
}
void Transforms::make_scaling(Subset* subset, float Sxyz){
  //---------------------------

  //Reverso old scaling
  mat4 scaling_reverse(1/subset->scale[0][0]);
  this->make_Transformation_atomic(subset->xyz, subset->COM, scaling_reverse);

  //Scale to new value
  mat4 scaling(Sxyz);
  subset->scale = scaling;
  this->make_Transformation_atomic(subset->xyz, subset->COM, scaling);

  //---------------------------
}

void Transforms::make_translation(vector<vec3>& XYZ, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //Apply
  this->make_Transformation_atomic(XYZ, vec3(0, 0, 0), translation);

  //---------------------------
}
void Transforms::make_rotation(vector<vec3>& XYZ, vec3 radian){
  //Rotation matrice creation - rx, ry, rz are in radian !
  glm::mat4 Rx(1.0);
  glm::mat4 Ry(1.0);
  glm::mat4 Rz(1.0);
  //---------------------------

  float rx = radian.x;
  float ry = radian.y;
  float rz = radian.z;

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

  glm::mat4 rotation = Rx * Ry * Rz;

  //Apply
  vec3 COM = fct_centroid(XYZ);
  this->make_Transformation_atomic(XYZ, COM, rotation);

  //---------------------------
}
void Transforms::make_rotation_origin(vector<vec3>& XYZ, mat4 R){
  //---------------------------

  vec3 COM = vec3(0, 0, 0);
  this->make_Transformation_atomic(XYZ, COM, R);

  //---------------------------
}

void Transforms::make_Transformation(Subset* subset, vec3 COM, mat4 transfMat){
  vector<vec3>& XYZ = subset->xyz;
  vector<vec3>& N = subset->N;
  vec3& ROOT = subset->root;
  //---------------------------

  this->make_Transformation_point(ROOT, COM, transfMat);
  this->make_Transformation_atomic(XYZ, COM, transfMat);
  //this->make_Transformation_normal(N, transfMat);

  //---------------------------
  this->compute_transformMatrix(subset, COM, transfMat);
}
void Transforms::make_Transformation_atomic(vector<vec3>& XYZ, vec3 COM, mat4 Transformation){
  //---------------------------

  //#pragma omp parallel for
  for(int i=0; i<XYZ.size(); i++){
    vec4 XYZ_hom = vec4(XYZ[i].x - COM.x, XYZ[i].y - COM.y, XYZ[i].z - COM.z, 1.0);
    vec4 XYZ_tr = XYZ_hom * Transformation;

    XYZ[i] = vec3(XYZ_tr.x + COM.x, XYZ_tr.y + COM.y, XYZ_tr.z + COM.z);
  }

  //---------------------------
}
void Transforms::make_Transformation_point(vec3& XYZ, vec3 COM, mat4 Transformation){
  //---------------------------

  for(int j=0;j<3;j++) XYZ[j] -= COM[j];
  vec4 newPos = vec4(XYZ, 1.0) * Transformation;
  for(int j=0;j<3;j++) XYZ[j] = newPos[j] + COM[j];

  //---------------------------
}
void Transforms::make_Transformation_normal(vector<vec3>& N, mat4 Transformation){
  mat3 rotMat = mat3(Transformation);
  //---------------------------

  #pragma omp parallel for
  for(int i=0;i<N.size();i++){
    N[i] = N[i] * rotMat;
  }

  //---------------------------
}

void Transforms::make_cloud_rotation(Cloud* cloud, vec3 R, string direction){
  //---------------------------

  this->compute_COM(cloud);

  if(direction == "up"){
    this->make_rotation(cloud, cloud->COM, R);
  }
  else if(direction == "down"){
    this->make_rotation(cloud, cloud->COM, -R);
  }

  //---------------------------
}

//Specific transformation functions
void Transforms::make_centering(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vec3 COM = subset->COM;
  //---------------------------

  //Cancel Center of mass from actual position
  this->make_translation(cloud, vec3(-COM.x,-COM.y,-COM.z));

  //Set min(Z) as corresponding to 0
  vec3 min = fct_min_vec3(subset->xyz);
  this->make_translation(cloud, vec3(0,0,-min[2]));

  //---------------------------
  console.AddLog("#", "Point cloud centered");
}
void Transforms::make_elevation(Cloud* cloud, float Z){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  Subset* subset_init = *next(cloud->subset_init.begin(), cloud->ID_selected);
  //---------------------------

  vector<vec3>& XYZ = subset->xyz;
  vector<vec3>& XYZ_ini = subset_init->xyz;

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].z = XYZ_ini[i].z + Z;
  }

  //---------------------------
}
float Transforms::make_orientAxis_X(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  //---------------------------

  //Retrieve A & B points
  vector<vec3>& XYZ = subset->xyz;
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
  this->make_rotation(cloud, subset->COM, rotation);

  //---------------------------
  return angle;
}
void Transforms::make_alignAxis_X(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vector<vec3>& XYZ = subset->xyz;
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].x = 0;
  }

  //---------------------------
}
float Transforms::make_alignAxisX_AB(Cloud* cloud, vec3 A, vec3 B){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  //---------------------------

  //Determination of angle with x axis
  float dot = B.x - A.x;
  float det = B.y - A.y;
  float angle = atan2(det, dot);

  //Align with x axis
  vec3 rotation = vec3(0, 0, angle);
  this->make_rotation(cloud, subset->COM, rotation);

  //---------------------------
  return angle;
}
float Transforms::make_alignAxisY_AB(Cloud* cloud, vec3 A, vec3 B){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  //---------------------------

  //Determination of angle with x axis
  float dot = B.x - A.x;
  float det = -(B.y - A.y);
  float angle = atan2(det, dot);

  //Align with x axis
  vec3 rotation = vec3(0, 0, angle);
  this->make_rotation(cloud, subset->COM, rotation);

  //---------------------------
  return angle;
}

//Position functions
void Transforms::make_positionning(Cloud* cloud, vec3 pos){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vec3& COM = subset->COM;
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = pos.z - COM.z;

  //---------------------------
  this->make_translation(cloud, diff);
}
void Transforms::make_positionning_XY(Cloud* cloud, vec3 pos){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vec3& COM = subset->COM;
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = 0;

  //---------------------------
  this->make_translation(cloud, diff);
}
void Transforms::make_positionning_glyph(vector<vec3>& XYZ, vec3& COM, vec3 pos){
  //---------------------------

  vec3 diff;
  diff.x = pos.x - COM.x;
  diff.y = pos.y - COM.y;
  diff.z = pos.z - COM.z;

  glm::mat4 Tmat(1.0);
  Tmat[0][3] = diff.x;
  Tmat[1][3] = diff.y;
  Tmat[2][3] = diff.z;

  //Application of the Transformation
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

//Operations
float Transforms::fct_soilDetermination(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vector<vec3>& XYZ = subset->xyz;
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
void Transforms::fct_adjustPosToScanner(Cloud* cloud, float Z_scan){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vector<vec3>& XYZ = subset->xyz;
  vec3& min = subset->min;
  float Z_soil = fct_soilDetermination(cloud);
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    XYZ[i].z = XYZ[i].z - Z_soil - Z_scan;
  }

  //---------------------------
}
vec3 Transforms::fct_degreeToRadian(vec3 degree){
  vec3 radian;
  //---------------------------

  for(int i=0; i<3; i++){
    radian[i] = degree[i] * M_PI / 180;
  }

  //---------------------------
  return radian;
}
mat4 Transforms::compute_transformMatrix(vec3 trans, vec3 rotat, vec3 scale){
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
mat4 Transforms::compute_transformMatrix(float tx, float ty, float tz, float rx, float ry, float rz){
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
mat4 Transforms::compute_transformMatrix(Subset* subset, vec3 COM, mat4 transformation){
  mat4& transMat = subset->transformation;
  //---------------------------

  transMat[0][3] -= COM.x;
  transMat[1][3] -= COM.y;
  transMat[2][3] -= COM.z;

  transMat *= transformation;

  transMat[0][3] += COM.x;
  transMat[1][3] += COM.y;
  transMat[2][3] += COM.z;

  //---------------------------
  return transMat;
}
vec3 Transforms::compute_anglesError(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  mat4 matReal = subset->transformation;//.RealTransformation;
  mat4 matIcp = subset->transformation;
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
vec3 Transforms::compute_translationsError(Cloud* cloud){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  mat4 matReal = subset->transformation; //.RealTransformation;
  mat4 matIcp = subset->transformation;
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
vec3 Transforms::compute_anglesFromTransformationMatrix(const mat4& mat){
  vec3 angles;
  //---------------------------

  float ax = atan2(mat[2][1], mat[2][2]);
  float ay = atan2(-mat[2][0], sqrt( pow(mat[2][1], 2) + pow(mat[2][2], 2) ) );
  float az = atan2(mat[1][0], mat[0][0]);

  angles = vec3(ax, ay, az);

  //---------------------------
  return angles;
}
vec3 Transforms::compute_anglesFromTransformationMatrix(const Eigen::Matrix3f& mat){
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
vec3 Transforms::compute_anglesFromTransformationMatrix(const Eigen::Matrix3d& mat){
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
vec3 Transforms::compute_translFromTransformationMatrix(const mat4& mat){
  vec3 translation;
  //---------------------------

  float tx = mat[0][3];
  float ty = mat[1][3];
  float tz = mat[2][3];

  translation = vec3(tx, ty, tz);

  //---------------------------
  return translation;
}
vector<vec3> Transforms::compute_transformcloud_XYZ(Cloud* cloud, mat4 Mat){
  Subset* subset = *next(cloud->subset.begin(), cloud->ID_selected);
  vec3& COM = subset->COM;
  vector<vec3> XYZ = subset->xyz;
  //---------------------------

  //Application of the Transformation
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
void Transforms::compute_transformXYZ(vector<vec3>& XYZ, vec3& COM, mat4 Mat){
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
void Transforms::compute_COM(Cloud* cloud){
  //---------------------------

  if(cloud->nb_subset == 1){
    Subset* subset = *next(cloud->subset.begin(), 0);
    cloud->COM = fct_centroid(subset->xyz);
  }

  //---------------------------
}
