#ifdef WITH_PCL
#include "Keypoint.h"

#include "../../../../Engine/Glyphs.h"

#include "pcl/features/pfh.h"
#include <pcl/keypoints/sift_keypoint.h>
#include <pcl/keypoints/harris_3d.h>
#include <pcl/keypoints/susan.h>
#include <pcl/features/shot_omp.h>
#include <pcl/features/3dsc.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/registration/transformation_estimation.h>
#include <pcl/registration/correspondence_rejection_sample_consensus.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/registration/correspondence_estimation.h>
#include <pcl/correspondence.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/segmentation/sac_segmentation.h>


//Constructor / Destructor
Keypoint::Keypoint(Glyphs* glyph){
  //---------------------------

  this->glyphManager = glyph;

  this->down_VoxelLeaf = 0.4f;
  this->HARRIS_threshold = 0.00002;
  this->descriptor_radius = 0.30;
  this->keypoint_radius = 0.000;
  this->show_sphereAtKeypoint = false;
  this->sphere_colored = true;

  this->reject_correspRANSAC = true;
  this->reject_correspRANSAC_iterMax = 1000;
  this->reject_correspSCORE = true;
  this->reject_correspNbBestON = false;
  this->reject_correspNbBest = 10;

  this->reject_siftSCORE = true;
  this->reject_siftNbBestON = false;
  this->reject_siftNbBest = 100;
  this->reject_siftNN = false;

  this->SIFT_minScale = 0.004;
  this->SIFT_minContrast = 0; //4;
  this->SIFT_nbOctaves = 4;
  this->SIFT_nbScalePerOctave = 4;

  this->normalMet = 0;
  this->downMet = 0;
  this->keyMeth = 1;
  this->desMet = 1;
  this->rejMet = 0;
  this->dataMet = 1;

  //---------------------------
}
Keypoint::~Keypoint(){}

//Main functions
void Keypoint::algo_keypoints(Collection* cloud_P, Collection* cloud_Q){
  corresp_P.clear(); corresp_Q.clear();
  correspondence_scores.clear();
  //---------------------------

  //Data
  switch(dataMet){
    case 0:{ //RGB ini
      if(cloud_P->color.hasData == false && cloud_Q->color.hasData == false){
        console.AddLog("[error] No color data in one cloud");
        return;
      }

      console.AddLog("keypoints based color...");

      cloud_P->pcl.XYZRGB = glm_XYZRGBinit_to_pcl_XYZRGB(cloud_P);
      cloud_Q->pcl.XYZRGB = glm_XYZRGBinit_to_pcl_XYZRGB(cloud_Q);

      /*for(int i=0; i<10;i++)
      console.AddLog("P : %i %i %i",cloud_P->pcl.XYZRGB->points[i].r,cloud_P->pcl.XYZRGB->points[i].g,cloud_P->pcl.XYZRGB->points[i].b);
      for(int i=0; i<10;i++)
      console.AddLog("Q : %i %i %i",cloud_Q->pcl.XYZRGB->points[i].r,cloud_Q->pcl.XYZRGB->points[i].g,cloud_Q->pcl.XYZRGB->points[i].b);*/

      break;
    }
    case 1:{ //I obj
      if(cloud_P->intensity.hasData == false || cloud_Q->intensity.hasData == false){
        console.AddLog("[error] No intensity data in one cloud");
        return;
      }

      console.AddLog("keypoints based intensity...");

      cloud_P->pcl.XYZRGB = glm_XYZIobj_to_pcl_XYZRGB(cloud_P);
      cloud_Q->pcl.XYZRGB = glm_XYZIobj_to_pcl_XYZRGB(cloud_Q);

      /*for(int i=0; i<10;i++)
      console.AddLog("P : %i %i %i",cloud_P->pcl.XYZRGB->points[i].r,cloud_P->pcl.XYZRGB->points[i].g,cloud_P->pcl.XYZRGB->points[i].b);
      for(int i=0; i<10;i++)
      console.AddLog("Q : %i %i %i",cloud_Q->pcl.XYZRGB->points[i].r,cloud_Q->pcl.XYZRGB->points[i].g,cloud_Q->pcl.XYZRGB->points[i].b);*/

      break;
    }
  }

  //DownSampling
  /*switch(downMet){
    case 0:{ //None
      cloud_P->pcl.Nxyz = glm_to_pcl_Nxyz(cloud_P);
      cloud_Q->pcl.Nxyz = glm_to_pcl_Nxyz(cloud_Q);
      break;
    }
    case 1:{ //Voxel grid
      this->downSamp_VoxelGrid(cloud_P);
      this->downSamp_VoxelGrid(cloud_Q);
      break;
    }
    case 2:{ //Segmentation
      this->downSamp_Segmentation(cloud_P);
      this->downSamp_Segmentation(cloud_Q);
      break;
    }
  }

  //Normals
  switch(normalMet){
    case 0:{// Pre-computed
      break;
    }
    case 1:{ //PCL estimation
      this->algo_normals(cloud_P);
      this->algo_normals(cloud_Q);
    }
  }

  //Keypoint
  switch(keyMeth){
    case 0:{ //None
      break;
    }
    case 1:{ //SIFT
      this->remove_pointAtKeypoint();

      cloud_P->registration.keypoints.clear();
      cloud_Q->registration.trgpoints.clear();

      this->keypoint_SIFT(cloud_P);
      this->keypoint_SIFT(cloud_Q);
      break;
    }
    case 2:{ //HARRIS
      this->remove_pointAtKeypoint();

      cloud_P->registration.keypoints.clear();
      cloud_Q->registration.trgpoints.clear();

      this->keypoint_HARRIS3D(cloud_P);
      this->keypoint_HARRIS3D(cloud_Q);
      break;
    }
    case 3:{ //SUSAN
      this->remove_pointAtKeypoint();

      cloud_P->registration.keypoints.clear();
      cloud_Q->registration.trgpoints.clear();

      this->keypoint_SUSAN(cloud_P);
      this->keypoint_SUSAN(cloud_Q);
      break;
    }
  }

  //Descriptor
  switch(desMet){
    case 0:{ //None
      break;
    }
    case 1:{ //SHOT
      SHOTtype descrip_P (new pcl::PointCloud<pcl::SHOT1344>);
      SHOTtype descrip_Q (new pcl::PointCloud<pcl::SHOT1344>);

      this->descriptor_SHOT(cloud_P, descrip_P);
      this->descriptor_SHOT(cloud_Q, descrip_Q);
      this->compute_correspondences_SHOT(descrip_P, descrip_Q);
      break;
    }
    case 2:{ //FPFH
      FPFHtype descrip_P (new pcl::PointCloud<pcl::FPFHSignature33>);
      FPFHtype descrip_Q (new pcl::PointCloud<pcl::FPFHSignature33>);

      this->descriptor_FPFH(cloud_P, descrip_P);
      this->descriptor_FPFH(cloud_Q, descrip_Q);
      this->compute_correspondences_FPFH(descrip_P, descrip_Q);
      break;
    }
    default:{
      break;
    }
  }

  //Correspondance rejection
  if(reject_correspSCORE){
    this->rejection_correspScore();
  }
  if(reject_correspNbBestON){
    this->rejection_correspNbBest();
  }
  if(reject_correspRANSAC){
    this->rejection_Ransac(cloud_P, cloud_Q);
  }

  //---------------------------
  this->compute_matching(cloud_P, cloud_Q);*/
}
void Keypoint::algo_keypoints_one(Collection* collection){
  /*cloud->pcl.Nxyz = glm_to_pcl_Nxyz(cloud);
  //---------------------------

  //Data
  switch(dataMet){
    case 0:{ //RGB init
      cloud->pcl.XYZRGB = glm_XYZRGBinit_to_pcl_XYZRGB(cloud);
      sphere_colored = false;
      break;
    }
    case 1:{ //I obj
      if(cloud->intensity.hasData == false){
        console.AddLog("[error] No intensity data");
        return;
      }
      cloud->pcl.XYZRGB = glm_XYZIobj_to_pcl_XYZRGB(cloud);
      sphere_colored = true;
      break;
    }
  }

  //DownSampling
  switch(downMet){
    case 0:{ //None
      break;
    }
    case 1:{ //Voxel grid
      this->downSamp_VoxelGrid(cloud);
      break;
    }
    case 2:{ //Segmentation
      this->downSamp_Segmentation(cloud);
      break;
    }
  }

  //Keypoint
  switch(keyMeth){
    case 0:{ //None
      break;
    }
    case 1:{ //SIFT
      this->remove_pointAtKeypoint();
      this->keypoint_SIFT(cloud);
      break;
    }
    case 2:{ //HARRIS
      this->remove_pointAtKeypoint();
      this->keypoint_HARRIS3D(cloud);
      break;
    }
    case 3:{ //SUSAN
      this->remove_pointAtKeypoint();
      this->keypoint_SUSAN(cloud);
      break;
    }
  }

  //Descriptor
  switch(desMet){
    case 0:{ //None
      break;
    }
    case 1:{ //SHOT
      SHOTtype descrip (new pcl::PointCloud<pcl::SHOT1344>);
      this->descriptor_SHOT(cloud, descrip);
      break;
    }
    case 2:{ //FPFH
      FPFHtype descrip (new pcl::PointCloud<pcl::FPFHSignature33>);
      this->descriptor_FPFH(cloud, descrip);
      break;
    }
    default:{
      break;
    }
  }

  //---------------------------*/
}
void Keypoint::algo_normals(Collection* collection){
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud = cloud->pcl.XYZRGB;
  pcl::NormalEstimationOMP<pcl::PointXYZRGB, pcl::Normal> ne;
  pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree_n(new pcl::search::KdTree<pcl::PointXYZRGB>());
  //---------------------------

  ne.setInputCloud(cloud);
  ne.setSearchMethod(tree_n);
  ne.setRadiusSearch(descriptor_radius);
  ne.compute(*normals);

  //---------------------------
  cloud->pcl.Nxyz = normals;
}

//DownSampling
void Keypoint::downSamp_VoxelGrid(Collection* collection){
  XYZRGBNtype cloud = glm_to_pcl_XYZRGBNormal(cloud);
  cout<<"Voxel grid: "<<cloud->size()<<flush;
  //---------------------------

  //Cloud
  pcl::VoxelGrid<pcl::PointXYZRGBNormal> sor;
  sor.setDownsampleAllData(true);
  sor.setInputCloud (cloud);
  sor.setLeafSize (down_VoxelLeaf, down_VoxelLeaf, down_VoxelLeaf);
  sor.filter (*cloud);

  cout<<"->"<<cloud->size()<<endl;
  cloud->pcl.XYZRGB->resize(cloud->size());
  pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
  normals->resize(cloud->size());
  for(int i=0; i<cloud->size(); i++){
    cloud->pcl.XYZRGB->points[i].x = cloud->points[i].x;
    cloud->pcl.XYZRGB->points[i].y = cloud->points[i].y;
    cloud->pcl.XYZRGB->points[i].z = cloud->points[i].z;

    normals->points[i].normal_x = cloud->points[i].normal_x;
    normals->points[i].normal_y = cloud->points[i].normal_y;
    normals->points[i].normal_z = cloud->points[i].normal_z;
  }
  cloud->pcl.Nxyz = normals;

  //---------------------------
}
void Keypoint::downSamp_Octree(Collection* collection){
  /*XYZRGBNtype cloud = glm_to_pcl_XYZRGBNormal(cloud);
  cout<<"Voxel grid: "<<cloud->size()<<flush;
  //---------------------

  float leaf = 0.5;
  pcl::octree::OctreePointCloudVoxelCentroid octree(leaf);
  octree.setInputCloud(cloud);
  octree.defineBoundingBox();
  octree.addPointsFromInputCloud();

  pcl::octree::OctreePointCloud::AlignedPointTVector centroids;
  octree.getVoxelCentroids(centroids);




  Cloud::Ptr output(new Cloud);
  output->points.assign(centroids.begin(), centroids.end());
  output->width = uint32_t(centroids.size());
  output->height = 1;
  output->is_dense = true;

  //---------------------
  cout<<"->"<<cloud->size()<<endl;
  cloud->pcl.XYZRGB->resize(cloud->size());
  pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
  normals->resize(cloud->size());
  for(int i=0; i<cloud->size(); i++){
    cloud->pcl.XYZRGB->points[i].x = cloud->points[i].x;
    cloud->pcl.XYZRGB->points[i].y = cloud->points[i].y;
    cloud->pcl.XYZRGB->points[i].z = cloud->points[i].z;

    normals->points[i].normal_x = cloud->points[i].normal_x;
    normals->points[i].normal_y = cloud->points[i].normal_y;
    normals->points[i].normal_z = cloud->points[i].normal_z;
  }
  cloud->pcl.Nxyz = normals;*/
}
void Keypoint::downSamp_Segmentation(Collection* collection){
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  cout<<"Segmentation: "<<cloud->width * cloud->height<<flush;
  //---------------------------

  // fit plane and keep points above that plane
  pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
  pcl::PointIndices::Ptr inliers(new pcl::PointIndices);

  pcl::SACSegmentation<pcl::PointXYZRGB> seg;
  seg.setOptimizeCoefficients(true);
  seg.setModelType(pcl::SACMODEL_PLANE);
  seg.setMethodType(pcl::SAC_RANSAC);
  seg.setDistanceThreshold(0.02);
  seg.setInputCloud(cloud->pcl.XYZRGB);
  seg.segment(*inliers, *coefficients);

  pcl::ExtractIndices<pcl::PointXYZRGB> extract;
  extract.setInputCloud(cloud->pcl.XYZRGB);
  extract.setIndices(inliers);
  extract.setNegative(true);

  extract.filter(*cloud);
  std::vector<int> indices;
  pcl::removeNaNFromPointCloud(*cloud, *cloud, indices);

  //Clustering
  pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
  tree->setInputCloud(cloud);

  std::vector<pcl::PointIndices> cluster_indices;
  pcl::EuclideanClusterExtraction<pcl::PointXYZRGB> clustering;
  clustering.setClusterTolerance(0.02);
  clustering.setMinClusterSize(1000);
  clustering.setMaxClusterSize(250000);
  clustering.setSearchMethod(tree);
  clustering.setInputCloud(cloud);
  clustering.extract(cluster_indices);

  if(cluster_indices.empty()){
    console.AddLog("[error] No cluster found");
    return;
  }else{
    cout << cluster_indices.size() << " clusters found" << endl;
  }

  pcl::IndicesPtr idx(new std::vector<int>);
  *idx = cluster_indices[0].indices;

  extract.setInputCloud(cloud);
  extract.setIndices(idx);
  extract.setNegative(false);
  extract.filter(*cloud);

  //---------------------------
  cout<<"->"<<cloud->width * cloud->height<<endl;
}

//Keypoints
void Keypoint::keypoint_SIFT(Collection* collection){
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  PtWithScale keypoints (new pcl::PointCloud<pcl::PointWithScale>);
  tic();
  //---------------------------

  // Compute keypoints
  pcl::SIFTKeypoint<pcl::PointXYZRGB, pcl::PointWithScale> sift;
  sift.setSearchMethod(pcl::search::KdTree<pcl::PointXYZRGB>::Ptr(new pcl::search::KdTree<pcl::PointXYZRGB>));
  sift.setScales (SIFT_minScale, SIFT_nbOctaves, SIFT_nbScalePerOctave);
  sift.setMinimumContrast (SIFT_minContrast);
  sift.setInputCloud (cloud);
  sift.compute (*keypoints);

  //Convert keypoints into XYZRGB
  XYZRGBtype keypoints_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::copyPointCloud (*keypoints, *keypoints_xyzrgb);

  //Reject keypoints which are too close from each other
  if(reject_siftNN){
    this->rejection_siftNNradius(keypoints_xyzrgb);
  }
  // Draw each keypoint as a sphere
  if(show_sphereAtKeypoint){
    this->compute_pointAtKeypoint(keypoints_xyzrgb);
  }

  cloud->pcl.keypoints = keypoints_xyzrgb;

  //---------------------------
  float duration = toc();
  int size = keypoints->size();
  console.AddLog("SIFT %s: Found %i keypoints in %.2fms", cloud->name.c_str(), size, duration);
}
void Keypoint::keypoint_HARRIS3D(Collection* collection){
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZItype keypoints (new pcl::PointCloud<pcl::PointXYZI>);
  //---------------------------

  // Compute keypoints
  pcl::HarrisKeypoint3D<pcl::PointXYZRGB, pcl::PointXYZI> harris;
  harris.setSearchMethod(pcl::search::KdTree<pcl::PointXYZRGB>::Ptr(new pcl::search::KdTree<pcl::PointXYZRGB>));
  harris.setNonMaxSupression (true);
  harris.setRefine(false);
  harris.setThreshold (HARRIS_threshold);
  harris.setInputCloud (cloud);
  harris.compute (*keypoints);

  //Convert keypoints into XYZRGB
  XYZRGBtype keypoints_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::copyPointCloud (*keypoints, *keypoints_xyzrgb);

  // Draw each keypoint as a sphere
  if(show_sphereAtKeypoint){
    this->compute_pointAtKeypoint(keypoints_xyzrgb);
  }

  //---------------------------
  cloud->pcl.keypoints = keypoints_xyzrgb;
  int size = keypoints->size();
  int size_tot = cloud->size();
  console.AddLog("HARRIS %s: Found %i keypoints out of %i total points.", cloud->name.c_str(), size, size_tot);
}
void Keypoint::keypoint_HARRIS6D(Collection* collection){
  /*XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZItype keypoints (new pcl::PointCloud<pcl::PointXYZI>);
  //-----------------------

  // Compute keypoints
  pcl::HarrisKeypoint6D<pcl::PointXYZRGB, pcl::PointXYZI> harris;
  harris.setSearchMethod(pcl::search::KdTree<pcl::PointXYZRGB>::Ptr(new pcl::search::KdTree<pcl::PointXYZRGB>));
  harris.setNonMaxSupression (true);
  harris.setRefine(false);
  harris.setRadius( 0.03f );
  harris.setRadiusSearch( 0.03f );
  harris.setThreshold (HARRIS_threshold);
  harris.setInputCloud (cloud);
  harris.compute (*keypoints);

  //Convert keypoints into XYZRGB
  XYZRGBtype keypoints_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::copyPointCloud (*keypoints, *keypoints_xyzrgb);

  // Draw each keypoint as a sphere
  if(show_sphereAtKeypoint){
    this->compute_pointAtKeypoint(keypoints_xyzrgb);
  }

  //-----------------------
  cloud->pcl.keypoints = keypoints_xyzrgb;
  cout << "HARRIS " << cloud->name <<": Found " << keypoints->size () << " keypoints ";
  cout << "out of " << cloud->size () << " total points." << endl;*/
}
void Keypoint::keypoint_SUSAN(Collection* collection){
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  pcl::PointCloud<pcl::Normal>::Ptr &normals = cloud->pcl.Nxyz;
  PtWithScale keypoints (new pcl::PointCloud<pcl::PointWithScale>);
  //---------------------------

  // Compute keypoints
  pcl::SUSANKeypoint<pcl::PointXYZRGB, pcl::PointWithScale> susan;
  susan.setSearchMethod(pcl::search::KdTree<pcl::PointXYZRGB>::Ptr(new pcl::search::KdTree<pcl::PointXYZRGB>));
  susan.setInputCloud (cloud);
  susan.setNormals (normals);
  susan.setIntensityThreshold (0.01);
  susan.compute (*keypoints);

  //Convert keypoints into XYZRGB
  XYZRGBtype keypoints_xyzrgb (new pcl::PointCloud<pcl::PointXYZRGB>);
  pcl::copyPointCloud (*keypoints, *keypoints_xyzrgb);

  // Draw each keypoint as a sphere
  if(show_sphereAtKeypoint){
    this->compute_pointAtKeypoint(keypoints_xyzrgb);
  }

  //---------------------------
  cloud->pcl.keypoints = keypoints_xyzrgb;
  int size = keypoints->size();
  int size_tot = cloud->size();
  console.AddLog("SUSAN %s: Found %i keypoints out of %i total points.", cloud->name.c_str(), size, size_tot);
}

//Descriptors
void Keypoint::descriptor_PFH(Collection* collection, PFHtype& descriptors){
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  pcl::PointCloud<pcl::Normal>::Ptr &normals = cloud->pcl.Nxyz;
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZRGBtype& keypoints = cloud->pcl.keypoints;
  //---------------------------

  pcl::PFHEstimation<pcl::PointXYZRGB, pcl::Normal, pcl::PFHSignature125> pfh;
  pfh.setSearchMethod (pcl::search::KdTree<pcl::PointXYZRGB>::Ptr (new pcl::search::KdTree<pcl::PointXYZRGB>));
  pfh.setRadiusSearch (descriptor_radius);
  pfh.setSearchSurface (cloud);
  pfh.setInputNormals (normals);
  pfh.setInputCloud (keypoints);
  pfh.compute (*descriptors);

  //---------------------------
}
void Keypoint::descriptor_FPFH(Collection* collection, FPFHtype& descriptors){
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>());
  pcl::PointCloud<pcl::Normal>::Ptr &normals = cloud->pcl.Nxyz;
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZRGBtype& keypoints = cloud->pcl.keypoints;
  //---------------------------

  pcl::FPFHEstimationOMP<pcl::PointXYZRGB, pcl::Normal, pcl::FPFHSignature33> fpfh;
  fpfh.setSearchMethod(pcl::search::KdTree<pcl::PointXYZRGB>::Ptr (new pcl::search::KdTree<pcl::PointXYZRGB>));
  fpfh.setRadiusSearch(descriptor_radius);
  fpfh.setSearchSurface(cloud);
  fpfh.setInputNormals(normals);
  fpfh.setInputCloud(keypoints);
  fpfh.compute(*descriptors);

  //---------------------------
}
void Keypoint::descriptor_SHOT(Collection* collection, SHOTtype& descriptors){
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  pcl::PointCloud<pcl::Normal>::Ptr &normals = cloud->pcl.Nxyz;
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZRGBtype& keypoints = cloud->pcl.keypoints;
  tic();
  //---------------------------

  pcl::SHOTColorEstimationOMP<pcl::PointXYZRGB, pcl::Normal, pcl::SHOT1344> shot;
  shot.setSearchMethod (pcl::search::KdTree<pcl::PointXYZRGB>::Ptr (new pcl::search::KdTree<pcl::PointXYZRGB>));
  shot.setRadiusSearch (descriptor_radius);
  shot.setSearchSurface (cloud);
  shot.setInputNormals (normals);
  shot.setInputCloud (keypoints);
  shot.compute (*descriptors);

  //Supression of the nan descriptors
  SHOTtype desc_2 (new pcl::PointCloud<pcl::SHOT1344>);
  for(int i=0; i<descriptors->size(); i++){
    if(!isnan(descriptors->points[i].descriptor[0]) && !isnan(descriptors->points[i].rf[0])){
      desc_2->push_back(descriptors->points[i]);
    }
  }

  //---------------------------
  descriptors = desc_2;
  float duration = toc();
  int size = desc_2->size();
  console.AddLog("-> SHOT %s: vectorize %i in %.2fms", cloud->name.c_str(), size, duration);
}
void Keypoint::descriptor_3DSC(Collection* collection, SCtype& descriptors){
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ> ());
  pcl::PointCloud<pcl::Normal>::Ptr &normals = cloud->pcl.Nxyz;
  XYZRGBtype& cloud = cloud->pcl.XYZRGB;
  XYZRGBtype& keypoints = cloud->pcl.keypoints;
  const float radius_ = 0.08;
  //---------------------------

  pcl::ShapeContext3DEstimation<pcl::PointXYZRGB, pcl::Normal, pcl::ShapeContext1980> dsc;
  dsc.setSearchMethod (pcl::search::KdTree<pcl::PointXYZRGB>::Ptr (new pcl::search::KdTree<pcl::PointXYZRGB>));
  dsc.setRadiusSearch (radius_);
  dsc.setSearchSurface (cloud);
  dsc.setMinimalRadius(radius_/10.f);
  dsc.setPointDensityRadius(radius_/ 5.0);
  dsc.setInputNormals (normals);
  dsc.setInputCloud (keypoints);
  dsc.compute (*descriptors);

  //---------------------------
}

//Rejection
void Keypoint::rejection_siftScore(PtWithScale& keypoints){
  if(keypoints->size() == 0) return;
  int size_old = keypoints->size();
  //---------------------------

  //Calcul du score médian
  float median_score = 0;
  for(int i=0; i<keypoints->size(); i++){
    median_score += keypoints->points[i].response;
  }
  median_score /= keypoints->size();

  //Supress from score
  PtWithScale temp (new pcl::PointCloud<pcl::PointWithScale>);
  for(int i=0; i<keypoints->size(); i++){
    say(median_score);
    say(keypoints->points[i].response);
    if(keypoints->points[i].response >= median_score){
      const pcl::PointWithScale& p = keypoints->points[i];
      temp->push_back(p);
    }
  }

  //---------------------------
  keypoints = temp;
  int size = keypoints->size();
  console.AddLog("-> rejection SHOT score: %i -> %i", size_old, size);
}
void Keypoint::rejection_siftNbBest(PtWithScale& keypoints){
  if(keypoints->size() == 0) return;
  int size_old = keypoints->size();
  //---------------------------

  //Vector of keypoint score
  vector<float> score_temp;
  for(int i=0; i<keypoints->size(); i++){
    score_temp.push_back(keypoints->points[i].response);
  }

  //Supress from score
  PtWithScale key_temp (new pcl::PointCloud<pcl::PointWithScale>);
  int cpt = 0;
  for (auto i: fct_sortByIndexes_greater(score_temp)) {
    if(cpt == reject_siftNbBest) break;

    const pcl::PointWithScale& p = keypoints->points[i];
    key_temp->push_back(p);

    cpt ++;
  }

  //---------------------------
  keypoints = key_temp;
  int size = keypoints->size();
  console.AddLog("-> rejection SHOT nb best: %i -> %i", size_old, size);
}
void Keypoint::rejection_siftNNradius(XYZRGBtype& keypoints){
  vector<vec3> key_glm = pcl_XYZRGB_to_glm_vecXYZ(keypoints);
  vector<vec4> key_col = pcl_XYZRGB_to_glm_vecRGB(keypoints);
  int size_old = keypoints->size();
  //---------------------------

  //FLANN nearest neighbor
  flann::Matrix<float> key_flann = flann::Matrix<float>(&key_glm[0][0], key_glm.size(), 3);

  //Construct index
  flann::Index<flann::L2_3D<float>> index(key_flann, flann::KDTreeSingleIndexParams(50));
  index.buildIndex();

  //Do knn search
  vector<vector<int>> indices(key_flann.rows, vector<int>(1,0));
  vector<vector<float>> dists(key_flann.rows, vector<float>(1,0));

  flann::SearchParams params;
  params.checks = 10;
  params.cores = 0;
  index.radiusSearch(key_flann, indices, dists, keypoint_radius, params);

  //Space sampling filtering according to FLANN indices
  vector<vec3> key_glm_sampled;
  vector<vec4> key_col_sampled;
  vector<int> nn;

  for(int i=0; i<indices.size(); i++){
    bool isTooClose = false;

    //Check if the point i correspond to another closest point
    for(int j=0; j<nn.size(); j++){
      if(nn[j] == i){
        isTooClose = true;
      }
    }

    //If not the case save the point and get is neighborhoods
    if(isTooClose == false){
      key_glm_sampled.push_back(key_glm[i]);
      key_col_sampled.push_back(key_col[i]);

      for(int j=1; j<indices[i].size(); j++){
        nn.push_back(indices[i][j]);
      }
    }
  }

  //---------------------------
  keypoints = glm_XYZRGB_to_pcl_XYZRGB(key_glm_sampled, key_col_sampled);
  int size = keypoints->size();
  console.AddLog("-> rejection SHOT radius: %i -> %i", size_old, size);
}
void Keypoint::rejection_correspScore(){
  if(correspondence_scores.size() == 0) return;
  int size_old = correspondence_scores.size();
  //---------------------------

  //Calcul du score médian
  float median_score = 0;
  for(int i=0; i<correspondence_scores.size(); i++){
    median_score += correspondence_scores[i];
  }
  median_score /= correspondence_scores.size();

  //Supress according to score median
  vector<int> c_P, c_Q;
  for(int i=0; i<corresp_Q.size(); i++){
    if(correspondence_scores[i] > median_score){
      c_P.push_back(corresp_P[i]);
      c_Q.push_back(corresp_Q[i]);
    }
  }

  //---------------------------
  corresp_P = c_P;
  corresp_Q = c_Q;
  int size = corresp_Q.size();
  console.AddLog("-> rejection correspondence score %.2f: %i -> %i", median_score, size_old, size);
}
void Keypoint::rejection_correspNbBest(){
  if(correspondence_scores.size() == 0) return;
  std::vector<float> temp (correspondence_scores);
  cout<<"->rejection correspondence nb best: "<<corresp_Q.size()<<flush;
  //---------------------------

  vector<int> c_P, c_Q;
  int cpt = 0;
  for (auto i: fct_sortByIndexes_greater(temp)) {
    if(cpt == reject_correspNbBest) break;

    c_P.push_back(corresp_P[i]);
    c_Q.push_back(corresp_Q[i]);

    cpt ++;
  }

  //---------------------------
  corresp_P = c_P;
  corresp_Q = c_Q;
  cout<<"->"<<corresp_Q.size()<<endl;
}
void Keypoint::rejection_Ransac(Collection* cloud_P, Collection* cloud_Q){
  if(correspondence_scores.size() == 0) return;
  XYZRGBtype& key_P = cloud_P->pcl.keypoints;
  XYZRGBtype& key_Q = cloud_Q->pcl.keypoints;
  int size_old = corresp_Q.size();
  tic();
  //---------------------------

  pcl::CorrespondencesPtr correspondences (new pcl::Correspondences ());
  correspondences->resize(corresp_P.size());
  for(int i=0; i<corresp_Q.size(); i++) {
    (*correspondences)[i].index_query = corresp_P[i];
    (*correspondences)[i].index_match = corresp_Q[i];
  }

  pcl::registration::CorrespondenceRejectorSampleConsensus<pcl::PointXYZRGB> ransac;
  ransac.setInputSource(key_P);
  ransac.setInputTarget(key_Q);
  ransac.setMaximumIterations(reject_correspRANSAC_iterMax);
  ransac.setInputCorrespondences(correspondences);
  ransac.getCorrespondences(*correspondences);

  corresp_P.resize(correspondences->size());
  corresp_Q.resize(correspondences->size());
  for(int i=0; i<correspondences->size(); i++) {
    corresp_P[i] = (*correspondences)[i].index_query;
    corresp_Q[i] = (*correspondences)[i].index_match;
  }

  //---------------------------
  float duration = toc();
  int size = corresp_Q.size();
  console.AddLog("-> rejection RANSAC: %i -> %i in %.2fms", size_old, size, duration);
}

//Correspondances
void Keypoint::compute_correspondences_PFH(PFHtype& descrip_P, PFHtype& descrip_Q){
  pcl::search::KdTree<pcl::PFHSignature125> descriptor_kdtree;
  //---------------------------

  corresp_P.resize(descrip_P->size ());
  corresp_Q.resize(descrip_P->size ());
  correspondence_scores.resize(descrip_P->size());

  const int k = 1;
  std::vector<int> k_indices (k);
  std::vector<float> k_squared_distances (k);

  descriptor_kdtree.setInputCloud(descrip_Q);
  for (size_t i = 0; i < descrip_P->size (); ++i){
    descriptor_kdtree.nearestKSearch (*descrip_P, i, k, k_indices, k_squared_distances);
    corresp_P[i] = i;
    corresp_Q[i] = k_indices[0];
    correspondence_scores[i] = k_squared_distances[0];
  }

  //---------------------------
}
void Keypoint::compute_correspondences_SHOT(SHOTtype& descrip_P, SHOTtype& descrip_Q){
  if(descrip_P->size() == 0 || descrip_Q->size() == 0) return;
  tic();
  //---------------------------

  corresp_P.resize(descrip_P->size());
  corresp_Q.resize(descrip_P->size());
  correspondence_scores.resize(descrip_P->size());

  /*
  pcl::search::KdTree<pcl::SHOT1344> kdtree;
  kdtree.setInputCloud(descrip_Q);

  std::vector<int> id(1);
  std::vector<float> id_squared_distances(1);
  for(int i=0; i<descrip_P->size(); i++){
    kdtree.nearestKSearch(*descrip_P, i, 1, id, id_squared_distances);
    corresp_P[i] = i;
    corresp_Q[i] = id[0];
    correspondence_scores[i] = id_squared_distances[0];
  }

*/

  pcl::Correspondences all_correspondences;
  pcl::registration::CorrespondenceEstimation<pcl::SHOT1344, pcl::SHOT1344> est;
  est.setInputCloud (descrip_P);
  est.setInputTarget (descrip_Q);
  est.determineReciprocalCorrespondences (all_correspondences);

  for(int i=0; i<all_correspondences.size(); i++){
    corresp_P[i] = all_correspondences[i].index_query;
    corresp_Q[i] = all_correspondences[i].index_match;
    correspondence_scores[i] = all_correspondences[i].distance;
  }

  //---------------------------
  float duration = toc();
  int size = all_correspondences.size();
  console.AddLog("-> Correspondances: Found %i in %.2fms", size, duration);
}
void Keypoint::compute_correspondences_FPFH(FPFHtype& descrip_P, FPFHtype& descrip_Q){
  if(descrip_P->size() == 0 || descrip_Q->size() == 0) return;
  //---------------------------

  corresp_P.resize(descrip_P->size ());
  corresp_Q.resize(descrip_P->size ());
  correspondence_scores.resize(descrip_P->size ());

  pcl::search::KdTree<pcl::FPFHSignature33> descriptor_kdtree;
  descriptor_kdtree.setInputCloud(descrip_Q);

  const int k = 1;
  std::vector<int> k_indices (k);
  std::vector<float> k_squared_distances (k);
  for (size_t i = 0; i < descrip_P->size (); ++i){
    descriptor_kdtree.nearestKSearch (*descrip_P, i, k, k_indices, k_squared_distances);
    corresp_P[i] = i;
    corresp_Q[i] = k_indices[0];
    correspondence_scores[i] = k_squared_distances[0];
  }

  //---------------------------
}

//Visualization
void Keypoint::compute_matching(Collection* cloud_P, Collection* cloud_Q){
  XYZRGBtype& key_P = cloud_P->pcl.keypoints;
  XYZRGBtype& key_Q = cloud_Q->pcl.keypoints;
  vector<vec3>& keypoints_P = cloud_P->registration.keypoints;
  vector<vec3>& trgpoints_Q = cloud_Q->registration.trgpoints;

  keypoints_P.clear();
  trgpoints_Q.clear();
  //---------------------------

  for(int i=0; i<corresp_Q.size(); i++){
    int c_P = corresp_P[i];
    int c_Q = corresp_Q[i];

    pcl::PointXYZRGB& match_P = key_P->points[c_P];
    pcl::PointXYZRGB& match_Q = key_Q->points[c_Q];

    keypoints_P.push_back(vec3(match_P.x, match_P.y, match_P.z));
    trgpoints_Q.push_back(vec3(match_Q.x, match_Q.y, match_Q.z));
  }

  //---------------------------
}
void Keypoint::compute_pointAtKeypoint(XYZRGBtype& keyp){
  vector<vec3> pos;
  //---------------------------

  //Location
  for(size_t i=0; i<keyp->size(); ++i){
    const pcl::PointXYZRGB& p = keyp->points[i];
    pos.push_back(vec3(p.x, p.y, p.z));
  }

  //Color
  vec4 RGB;
  if(sphere_colored){
    RGB = vec4(0.1f,0.1f,1.0f,1.0f);
  }else{
    RGB = vec4(1.0f,1.0f,1.0f,1.0f);
  }

  float r = 5;
  string ID = glyphManager->obj_pointsAtLocation(pos, r, RGB);
  points_IDs.push_back(ID);

  //---------------------------
}
void Keypoint::remove_pointAtKeypoint(){
  //---------------------------

  for(int i=0; i<points_IDs.size(); i++){
    glyphManager->removeGlyph(points_IDs[i]);
  }
  points_IDs.clear();

  //---------------------------
}

#endif
