#ifdef WITH_PCL
#include "pcl_functions.h"

#include "pcl_transtypage.h"


//Constructor / Destructor
pcl_functions::pcl_functions(){
  //---------------------------

  this->radiusSearch = 0.03;

  //---------------------------
}
pcl_functions::~pcl_functions(){}

void pcl_functions::compute_normals_PCL(Cloud* cloud){
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud = glm_to_pcl_XYZ(cloud);
  tic();
  //---------------------------

  // Create the normal estimation class, and pass the input dataset to it
  pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> ne;
  pcl::PointCloud<pcl::Normal>::Ptr cloud_normals (new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());

  ne.setInputCloud (cloud);
  ne.setSearchMethod (tree);
  ne.setRadiusSearch (radiusSearch);

  // /!\ compute the normales with direction to the scanner position
  vec3& scanpos = cloud->root;
  ne.setViewPoint(scanpos.x, scanpos.y, scanpos.z);

  // Compute the features
  ne.compute (*cloud_normals);

  //Store computed normals into cloud data
  vec3 normal;
  cloud->Nxyz.clear();
  #pragma omp parallel for
  for(int i=0;i<cloud->xyz.size();i++){
    normal.x = cloud_normals->points[i].normal_x;
    normal.y = cloud_normals->points[i].normal_y;
    normal.z = cloud_normals->points[i].normal_z;

    cloud->Nxyz.push_back(normal);
  }

  //---------------------------
  float duration = toc();
  string log = "Normal for " + cloud->name + " computed in " + duration + " ms";
  console.AddLog("#", log);
}
void pcl_functions::Plane_cloud(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vector<vec4>& RGB = cloud->rgb;
  int size = cloud->nb_point;
  //---------------------------

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);

  pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
  pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
  pcl::SACSegmentation<pcl::PointXYZ> seg;
  seg.setOptimizeCoefficients(true);
  seg.setModelType(pcl::SACMODEL_PLANE);
  seg.setMethodType(pcl::SAC_RANSAC);
  seg.setDistanceThreshold(0.01);

  seg.setInputCloud(cloud_pcl);
  seg.segment(*inliers, *coefficients);

  vector<int> id;
  for(const auto& idx: inliers->indices){
      id.push_back(idx);
  }

  for(int i=0; i<id.size(); i++){
    RGB[id[i]] = vec4(1.0f, 0.0f, 0.0f, 0.0f);
  }

  //---------------------------
}
void pcl_functions::detectSphere(Cloud* cloud){
  vector<int> inliers;
  //---------------------------

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);
  pcl::PointCloud<pcl::PointXYZ>::Ptr final (new pcl::PointCloud<pcl::PointXYZ>);

  // created RandomSampleConsensus object and compute the appropriated model
  pcl::SampleConsensusModelSphere<pcl::PointXYZ>::Ptr
    model_s(new pcl::SampleConsensusModelSphere<pcl::PointXYZ> (cloud_pcl));
  pcl::RandomSampleConsensus<pcl::PointXYZ> ransac (model_s);
  ransac.setDistanceThreshold (0.001);
  ransac.computeModel();
  ransac.getInliers(inliers);

  pcl::copyPointCloud (*cloud_pcl, inliers, *final);
  Subset subset_out = pcl_to_glm_XYZ(final);
  cloud->xyz = subset_out.xyz;

  //---------------------------
}

#endif
