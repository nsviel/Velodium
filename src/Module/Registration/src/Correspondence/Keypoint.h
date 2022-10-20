#ifdef WITH_PCL
#ifndef KEYPOINT_H
#define KEYPOINT_H

#include "../../../../common.h"

class Glyphs;

typedef pcl::PointCloud<pcl::PFHSignature125>::Ptr PFHtype;
typedef pcl::PointCloud<pcl::FPFHSignature33>::Ptr FPFHtype;
typedef pcl::PointCloud<pcl::SHOT1344>::Ptr SHOTtype;
typedef pcl::PointCloud<pcl::ShapeContext1980>::Ptr SCtype;
typedef pcl::PointCloud<pcl::PointWithScale>::Ptr PtWithScale;
typedef pcl::PointCloud<pcl::PointXYZI>::Ptr XYZItype;
typedef pcl::PointCloud<pcl::PointXYZRGB>::Ptr XYZRGBtype;
typedef pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr XYZRGBNtype;


class Keypoint
{
public:
  //Constructor / Destructor
  Keypoint(Glyphs* glyph);
  ~Keypoint();

public:
  //Main functions
  void algo_keypoints(Cloud* cloud_P, Cloud* cloud_Q);
  void algo_keypoints_one(Cloud* cloud);
  void algo_normals(Cloud* cloud);

  //DownSampling
  void downSamp_VoxelGrid(Cloud* cloud);
  void downSamp_Segmentation(Cloud* cloud);
  void downSamp_Octree(Cloud* cloud);

  //Keypoints methods
  void keypoint_SIFT(Cloud* cloud);
  void keypoint_HARRIS3D(Cloud* cloud);
  void keypoint_HARRIS6D(Cloud* cloud);
  void keypoint_SUSAN(Cloud* cloud);

  //Descriptor methods
  void descriptor_PFH(Cloud* cloud, PFHtype& descriptors);
  void descriptor_FPFH(Cloud* cloud, FPFHtype& descriptors);
  void descriptor_SHOT(Cloud* cloud, SHOTtype& descriptors);
  void descriptor_3DSC(Cloud* cloud, SCtype& descriptors);

  //Rejection mesthods
  void rejection_siftNNradius(XYZRGBtype& keypoints);
  void rejection_siftScore(PtWithScale& keypoints);
  void rejection_siftNbBest(PtWithScale& keypoints);
  void rejection_correspScore();
  void rejection_correspNbBest();
  void rejection_Ransac(Cloud* cloud_P, Cloud* cloud_Q);

  void compute_correspondences_PFH(PFHtype& descrip_P, PFHtype& descrip_Q);
  void compute_correspondences_FPFH(FPFHtype& descrip_P, FPFHtype& descrip_Q);
  void compute_correspondences_SHOT(SHOTtype& descrip_P, SHOTtype& descrip_Q);
  void compute_matching(Cloud* cloud_P, Cloud* cloud_Q);
  void compute_pointAtKeypoint(XYZRGBtype& keyp);
  void remove_pointAtKeypoint();

  inline float* get_Down_VoxelLeaf(){return &down_VoxelLeaf;}
  inline float* get_SIFT_minScale(){return &SIFT_minScale;}
  inline float* get_SIFT_minContrast(){return &SIFT_minContrast;}
  inline float* get_HARRIS_threshold(){return &HARRIS_threshold;}
  inline float* get_descriptor_radius(){return &descriptor_radius;}
  inline float* get_keypoint_radius(){return &keypoint_radius;}

  inline bool* get_showSphereAtKeypoint(){return &show_sphereAtKeypoint;}
  inline bool* get_rejectCorrespRANSAC(){return &reject_correspRANSAC;}
  inline bool* get_rejectCorrespSCORE(){return &reject_correspSCORE;}
  inline bool* get_rejectCorrespNbBestON(){return &reject_correspNbBestON;}
  inline bool* get_rejectSiftSCORE(){return &reject_siftSCORE;}
  inline bool* get_rejectSiftNbBestON(){return &reject_siftNbBestON;}
  inline bool* get_rejectSiftRadiusLimit(){return &reject_siftNN;}

  inline int* get_SIFT_nbOctaves(){return &SIFT_nbOctaves;}
  inline int* get_SIFT_nbScalePerOctave(){return &SIFT_nbScalePerOctave;}
  inline int* get_rejectCorrespNbBest(){return &reject_correspNbBest;}
  inline int* get_rejectSiftNbBest(){return &reject_siftNbBest;}
  inline int* get_rejectRANSAC_iterMax(){return &reject_correspRANSAC_iterMax;}
  inline int* get_keypMethod(){return &keyMeth;}
  inline int* get_descMethod(){return &desMet;}
  inline int* get_downMethod(){return &downMet;}
  inline int* get_dataMethod(){return &dataMet;}
  inline int* get_normalMethod(){return &normalMet;}

private:
  Glyphs* glyphManager;

  float down_VoxelLeaf;
  float SIFT_minScale;
  float SIFT_minContrast;
  float descriptor_radius;
  float keypoint_radius;
  int SIFT_nbOctaves;
  int SIFT_nbScalePerOctave;
  int reject_correspNbBest, reject_siftNbBest;
  int reject_correspRANSAC_iterMax;
  float HARRIS_threshold;

  vector<string> points_IDs;
  vector<int> corresp_P, corresp_Q;
  vector<float> correspondence_scores;
  bool show_sphereAtKeypoint;
  bool reject_correspSCORE, reject_correspNbBestON;
  bool reject_siftNbBestON, reject_siftSCORE, reject_siftNN;
  bool reject_correspRANSAC;
  bool sphere_colored;

  int keyMeth, desMet, rejMet, downMet, dataMet, normalMet;
};

#endif
#endif
