#include "Transformation.h"

#include "../../Specific/Function/fct_math.h"


// Constructor / Destructor
Transformation::Transformation(){}
Transformation::~Transformation(){}

// Translation
void Transformation::make_translation(Collection* collection, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //Apply
  for(int i=0; i<collection->nb_obj; i++){
    Object_* object = *next(collection->list_obj.begin(), i);
    object->trans *= translation;
    this->make_Transformation(object, object->root, translation);
  }

  //---------------------------
}
void Transformation::make_translation(Object_* object, vec3 trans){
  //Translation matrice creation
  glm::mat4 translation(1.0);
  //---------------------------

  translation[0][3] = trans.x;
  translation[1][3] = trans.y;
  translation[2][3] = trans.z;

  //---------------------------
  object->trans *= translation;
  this->make_Transformation(object, object->root, translation);
}
void Transformation::make_translation(vector<vec3>& XYZ, vec3 trans){
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

// Rotation
mat4 Transformation::make_rotation(Collection* collection, vec3 COM, vec3 radian){
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
  for(int i=0; i<collection->nb_obj; i++){
    Object_* object = *next(collection->list_obj.begin(), i);
    object->rotat *= rotation;
    this->make_Transformation(object, COM, rotation);
  }

  //---------------------------
  return rotation;
}
void Transformation::make_rotation(Collection* collection, vec3 R, string direction){
  //---------------------------

  if(direction == "up"){
    this->make_rotation(collection, collection->COM, R);
  }
  else if(direction == "down"){
    this->make_rotation(collection, collection->COM, -R);
  }

  //---------------------------
}
void Transformation::make_rotation(Object_* object, vec3 COM, vec3 radian){
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
  object->rotat *= rotation;
  this->make_Transformation(object, COM, rotation);
}
void Transformation::make_rotation(vector<vec3>& XYZ, vec3 radian){
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
void Transformation::make_rotation_origin(vector<vec3>& XYZ, mat4 R){
  //---------------------------

  vec3 COM = vec3(0, 0, 0);
  this->make_Transformation_atomic(XYZ, COM, R);

  //---------------------------
}

// Scaling
void Transformation::make_scaling(Object_* object, float Sxyz){
  //---------------------------

  //Reverso old scaling
  mat4 scaling_reverse(1/object->scale[0][0]);
  scaling_reverse[3][3] = 1;
  this->make_Transformation_atomic(object->xyz, object->COM, scaling_reverse);

  //Scale to new value
  mat4 scaling(Sxyz);
  scaling[3][3] = 1;
  object->scale = scaling;
  this->make_Transformation_atomic(object->xyz, object->COM, scaling);

  //---------------------------
}

// Transformation
void Transformation::make_Transformation(Object_* object, vec3 COM, mat4 M){
  vector<vec3>& XYZ = object->xyz;
  vector<vec3>& N = object->Nxyz;
  vec3& ROOT = object->root;
  //---------------------------

  this->make_Transformation_atomic(XYZ, COM, M);
  this->make_transformation_attribut(object, COM, M);
  this->make_Transformation_point(ROOT, COM, M);
  this->make_Transformation_normal(N, M);

  //---------------------------
}
void Transformation::make_Transformation_atomic(vector<vec3>& xyz, vec3 COM, mat4 transform){
  //---------------------------

  //#pragma omp parallel for
  for(int i=0; i<xyz.size(); i++){
    vec4 xyz_hom = vec4(xyz[i].x - COM.x, xyz[i].y - COM.y, xyz[i].z - COM.z, 1.0);
    vec4 xyz_tr = xyz_hom * transform;

    xyz[i] = vec3(xyz_tr.x + COM.x, xyz_tr.y + COM.y, xyz_tr.z + COM.z);
  }

  //---------------------------
}
void Transformation::make_Transformation_point(vec3& XYZ, vec3 COM, mat4 Transformation){
  //---------------------------

  for(int j=0;j<3;j++) XYZ[j] -= COM[j];
  vec4 newPos = vec4(XYZ, 1.0) * Transformation;
  for(int j=0;j<3;j++) XYZ[j] = newPos[j] + COM[j];

  //---------------------------
}
void Transformation::make_Transformation_normal(vector<vec3>& N, mat4 Transformation){
  mat3 rotMat = mat3(Transformation);
  //---------------------------

  #pragma omp parallel for
  for(int i=0;i<N.size();i++){
    N[i] = N[i] * rotMat;
  }

  //---------------------------
}
mat4 Transformation::make_transformation_attribut(Object_* object, vec3 COM, mat4 transformation){
  mat4& M = object->transformation;
  //---------------------------

  M[0][3] -= COM.x;
  M[1][3] -= COM.y;
  M[2][3] -= COM.z;

  M *= transformation;

  M[0][3] += COM.x;
  M[1][3] += COM.y;
  M[2][3] += COM.z;

  //---------------------------
  return M;
}
