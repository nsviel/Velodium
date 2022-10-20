#ifdef WITH_PCL
#ifndef REGISTRATION_STRUCT_H
#define REGISTRATION_STRUCT_H

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>


struct struct_registration{
  pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr XYZRGBN;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr XYZRGB;
  pcl::PointCloud<pcl::PointXYZI>::Ptr XYZI;
  pcl::PointCloud<pcl::Normal>::Ptr N;
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr keypoints;
};

#endif
#endif
