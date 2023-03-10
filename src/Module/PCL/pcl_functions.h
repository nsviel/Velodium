#ifdef WITH_PCL
#ifndef PCL_FUNCTIONS_H
#define PCL_FUNCTIONS_H

class Scene;
class Attribut;

#include "../../common.h"

#include <pcl/features/normal_3d.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/sample_consensus/sac_model_sphere.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/search/kdtree.h>
#include <pcl/common/io.h>


class pcl_functions
{
public:
  //Constructor / Destructor
  pcl_functions();
  ~pcl_functions();

public:
  void compute_normals_PCL(Cloud* cloud);
  void Plane_cloud(Cloud* cloud);
  void detectSphere(Cloud* cloud);

  inline void set_normalRadiusSeach(float value){this->radiusSearch = value;}

private:
  float radiusSearch;
};

#endif
#endif
