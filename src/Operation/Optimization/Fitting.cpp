#include "Fitting.h"

#include "../Node_operation.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Glyphs.h"

#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_transtypage.h"


//Constructor / Destructor
Fitting::Fitting(Node_operation* node_ope){
  //--------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();

  //--------------------------
}
Fitting::~Fitting(){}

//Sphere fitting
void Fitting::Sphere_cloudToCenter_all(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //--------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;
    this->Sphere_cloudToCenter(subset);
  }

  //--------------------------
}
void Fitting::Sphere_cloudToCenter(Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vector<float>& dist = subset->R;
  //---------------------------

  float dist_min = fct_min(dist);
  int size = XYZ.size();
  float r = 0.0695;
  vec3 Center;

  //Search for nearest point
  float distm, Xm, Ym, Zm;
  for (int i=0; i<size; i++){
    if(dist[i] == dist_min){
        distm = dist[i];
        Xm = XYZ[i].x;
        Ym = XYZ[i].y;
        Zm = XYZ[i].z;
    }
  }

  //Determine the center of the sphere
  Center = vec3(Xm + r * (Xm / distm), Ym + r * (Ym / distm), Zm + r * (Zm / distm));
  Center = Sphere_FindCenter(subset);

  //Add a ptMark cloud to the selected point
  //int ID = glyphManager->loadGlyph("../media/engine/Marks/sphere_FARO.pts", Center, "point", false, 3);
  //glyphManager->update_glyph_color(ID, vec3(1.0f, 0.0f, 0.0f));

  //---------------------------
}
vec3 Fitting::Sphere_FindCenter(Subset* subset){
  /* The return value is 'true' when the linear system of the algorithm
   is solvable, 'false' otherwise. If 'false' is returned, the sphere
   center and radius are set to zero values.*/
   vector<vec3>& XYZ = subset->xyz;
   vec3 COM = subset->COM;
   vec3 Center;
   int numPoints = XYZ.size();
   //------------------------

  // Compute the covariance matrix M of the Y[i] = X[i]-A and the
  // right-hand side R of the linear system M*(C-A) = R.
  float M00 = 0, M01 = 0, M02 = 0, M11 = 0, M12 = 0, M22 = 0;
  vec3 R = {0, 0, 0};
  for (int i=0; i<numPoints; i++){
     vec3 Y = XYZ[i] - COM;
     float Y0Y0 = Y[0] * Y[0];
     float Y0Y1 = Y[0] * Y[1];
     float Y0Y2 = Y[0] * Y[2];
     float Y1Y1 = Y[1] * Y[1];
     float Y1Y2 = Y[1] * Y[2];
     float Y2Y2 = Y[2] * Y[2];
     M00 += Y0Y0;
     M01 += Y0Y1;
     M02 += Y0Y2;
     M11 += Y1Y1;
     M12 += Y1Y2;
     M22 += Y2Y2;
     R += (Y0Y0 + Y1Y1 + Y2Y2) * Y;
  }
  R *= 0.5;

  // Solve the linear system M*(C-A) = R for the center C.
  float cof00 = M11 * M22 - M12 * M12;
  float cof01 = M02 * M12 - M01 * M22;
  float cof02 = M01 * M12 - M02 * M11;
  float det = M00 * cof00 + M01 * cof01 + M02 * cof02;
  if(det != 0.0f){
     float cof11 = M00 * M22 - M02 * M02;
     float cof12 = M01 * M02 - M00 * M12;
     float cof22 = M00 * M11 - M01 * M01;
     Center[0] = COM[0] + (cof00 * R[0] + cof01 * R[1] + cof02 * R[2]) / det;
     Center[1] = COM[1] + (cof01 * R[0] + cof11 * R[1] + cof12 * R[2]) / det;
     Center[2] = COM[2] + (cof02 * R[0] + cof12 * R[1] + cof22 * R[2]) / det;

     float rsqr = 0.0f;
     for (int i=0; i<numPoints; i++){
         vec3 delta = XYZ[i] - Center;
         rsqr += fct_dotProduct(delta, delta);
     }
     rsqr *= (1/XYZ.size());
     Radius = std::sqrt(rsqr);
  }
  else{
     Center = {0, 0, 0};
     Radius = 0;
  }

  //------------------------
  return Center;
}

//Plane fitting
void Fitting::Plane_cloud_all(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //--------------------------

  /*#ifdef PCL_FUNCTIONS_H
  pcl_functions pclManager;
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;
    pclManager.Plane_cloud(subset);
    sceneManager->update_subset_color(subset);
  }
  #endif*/

  //--------------------------
}
