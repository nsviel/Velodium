#ifdef WITH_PCL
/*#include "Sampling.h"

#include "pcl_transtypage.h"

#include "../../Operation/Transformation/Attribut.h"
#include "../../Scene/Data/Scene.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/Function/fct_terminal.h"


//Constructor / Destructor
Sampling::Sampling(){
  //---------------------------

  this->sceneManager = new Scene();
  this->attribManager = new Attribut();

  this->sampling_percentagePts = 50;
  this->sampling_radius = 0.1f;
  this->sampling_squareSize = 0.013f;
  this->sampling_std = 1.0f;

  //---------------------------
}
Sampling::~Sampling(){}

//Functions
void Sampling::sampling_random(Cloud* cloud){
  int size_before = cloud->nb_point;
  tic();
  //---------------------------

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);

  //Retrieve left number of points
  float leftPts = size_before * (float(sampling_percentagePts)/100);

  //Sampling
  pcl::RandomSample<pcl::PointXYZ> RS(true);
  RS.setInputCloud(cloud_pcl);
  RS.setSample(leftPts);
  RS.filter(*cloud_pcl);
  pcl::IndicesConstPtr idx = RS.getRemovedIndices();
  vector<int> indices = *idx.get();

  float duration = toc();
  int size_filtered = cloud->xyz.size();
  attribManager->make_supressPoints(cloud, indices);

  //---------------------------
  sceneManager->update_buffer_location(cloud);
  string log = "Random sampling " + cloud->name + " : " + size_before + " -> " + size_filtered + " points (" + duration + " ms)";
  console.AddLog("ok", log);
}
void Sampling::sampling_spaceRadius_PCL(Cloud* cloud){
  int size_before = cloud->nb_point;
  tic();
  //---------------------------

  //Filtering
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_pcl = glm_to_pcl_XYZI(cloud);
  pcl::VoxelGrid<pcl::PointXYZI> sor;
  sor.setInputCloud(cloud_pcl);
  sor.setDownsampleAllData(true);
  sor.setLeafSize(sampling_squareSize,sampling_squareSize,sampling_squareSize);
  sor.filter(*cloud_pcl);

  //Retrieve data
  Subset subset_filter = pcl_to_glm_XYZI(cloud_pcl);
  cloud->xyz = subset_filter.xyz;
  cloud->I = subset_filter.I;

  //Update cloud
  //sceneManager->update_subset_IntensityToColor(cloud);
  sceneManager->update_buffer_location(cloud);

  //---------------------------
  float duration = toc();
  int size_filtered = cloud->xyz.size();
  string log = "Space sampling " + cloud->name + " : " + size_before + " -> " + size_filtered + " points (" + duration + " ms)";
  console.AddLog("ok", log);
}
void Sampling::sampling_outlier(Cloud* cloud){
  int size_before = cloud->nb_point;
  tic();
  //---------------------------

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);

  //Filtering
  pcl::RadiusOutlierRemoval<pcl::PointXYZ> RO(true);
  RO.setInputCloud (cloud_pcl);
  RO.setRadiusSearch (sampling_radius); //0.1m radius search for neigbors
  RO.setMinNeighborsInRadius (1); //min number of neighbors
  RO.setNegative (true);
  RO.filter (*cloud_pcl);
  pcl::IndicesConstPtr idx = RO.getRemovedIndices();
  vector<int> indices = *idx.get();

  //---------------------------
  attribManager->make_supressPoints(cloud, indices);
  sceneManager->update_buffer_location(cloud);

  float duration = toc();
  int size_filtered = cloud->xyz.size();
  string log = "Filter outliers " + cloud->name + " : " + size_before + " -> " + size_filtered + " points (" + duration + " ms)";
  console.AddLog("ok", log);
}
void Sampling::sampling_statistical(Cloud* cloud){
  //http://pointclouds.org/documentation/tutorials/statistical_outlier.php
  int size_before = cloud->nb_point;
  tic();
  //---------------------------

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);

  //Filtering
  pcl::StatisticalOutlierRemoval<pcl::PointXYZ> SO(true);
  SO.setMeanK (8);
  SO.setStddevMulThresh(sampling_std);
  SO.filter (*cloud_pcl);
  pcl::IndicesConstPtr idx = SO.getRemovedIndices();
  vector<int> indices = *idx.get();

  //---------------------------
  attribManager->make_supressPoints(cloud, indices);
  sceneManager->update_buffer_location(cloud);

  float duration = toc();
  int size_filtered = cloud->xyz.size();
  string log = "Statistical sampling " + cloud->name + " : " + size_before + " -> " + size_filtered + " points (" + duration + " ms)";
  console.AddLog("ok", log);
}
void Sampling::sampling_spaceRadius(Cloud* cloud, float radius){
  tic();
  //---------------------------

  vector<vec3>& XYZ = cloud->xyz;
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  int size_before = cloud->nb_point;

  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_pcl = glm_to_pcl_XYZ(cloud);

  pcl::octree::OctreePointCloudSearch<pcl::PointXYZ> octree(radius);
  octree.setInputCloud(cloud_pcl);
  octree.defineBoundingBox(min[0], min[1], min[2], max[0], max[1], max[2]);
  octree.addPointsFromInputCloud();

  vector<int> id_neighbor, id_pt, id_supp;
  vector<float> ptDist;
  for(int i=0; i<XYZ.size(); i++){
    id_pt.push_back(0);
  }

  //0 : point nont traité
  //-1 : point à supprimer
  //1 : point à garder
  pcl::PointXYZ searchPoint;
  for(int i=0; i<XYZ.size(); i++){
    if(id_pt[i] == 0){
      id_neighbor.clear();
      id_pt[i] = 1;

      searchPoint.x = XYZ[i][0];
      searchPoint.y = XYZ[i][1];
      searchPoint.z = XYZ[i][2];

      octree.radiusSearch(searchPoint, radius, id_neighbor, ptDist);

      for(int j=0; j<id_neighbor.size(); j++){

        if(id_pt[id_neighbor[j]] == 0){
          id_pt[id_neighbor[j]] = -1;
          id_supp.push_back(id_neighbor[j]);
        }
      }
    }
  }

  //---------------------------
  sort(id_supp.begin(), id_supp.end());
  attribManager->make_supressPoints(cloud, id_supp);
  sceneManager->update_buffer_location(cloud);

  float duration = toc();
  int size_filtered = cloud->xyz.size();
  string log = "Space sampling " + cloud->name + " : " + size_before + " -> " + size_filtered + " points (" + duration + " ms)";
  console.AddLog("ok", log);
}*/

#endif
