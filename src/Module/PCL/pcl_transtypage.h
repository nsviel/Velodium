#ifdef WITH_PCL
#ifndef TRANSTYPAGE_PCL_H
#define TRANSTYPAGE_PCL_H

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

/**
 * \namespace Transtypage
 * \brief Function to convert data format between libraries
 */

namespace{

  //GLM to PCL
  pcl::PointCloud<pcl::PointXYZ>::Ptr glm_to_pcl_XYZ(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::Normal>::Ptr glm_to_pcl_N(Subset* subset_in){
    pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
    const std::vector<glm::vec3>& N = subset_in->N;
    int size = N.size();
    //---------------------------

    //Normal glm to pcl
    normals->points.resize(size);
    for(int i=0; i<size; i++){
      normals->points[i].normal_x = N[i].x;
      normals->points[i].normal_y = N[i].y;
      normals->points[i].normal_z = N[i].z;
    }

    //---------------------------
    return normals;
  }
  pcl::PointCloud<pcl::PointNormal>::Ptr glm_to_pcl_XYZN(Subset* subset_in){
    pcl::PointCloud<pcl::PointNormal>::Ptr cloud_out (new pcl::PointCloud<pcl::PointNormal>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    const std::vector<glm::vec3>& N = subset_in->N;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].normal_x = N[i].x;
      cloud_out->points[i].normal_y = N[i].y;
      cloud_out->points[i].normal_z = N[i].z;
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZI>::Ptr glm_to_pcl_XYZI(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZI>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    std::vector<float>& Is = subset_in->I;
    if(Is.size() == 0) std::cout<<"No intensity data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].intensity = Is[i];
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr glm_to_pcl_XYZRGBNormal(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZRGBNormal>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    const std::vector<glm::vec4>& RGB = subset_in->RGB;
    const std::vector<glm::vec3>& N = subset_in->N;
    if(RGB.size() == 0) std::cout<<"No color data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].r = (int)(RGB[i].x*255);
      cloud_out->points[i].g = (int)(RGB[i].y*255);
      cloud_out->points[i].b = (int)(RGB[i].z*255);

      cloud_out->points[i].normal_x = N[i].x;
      cloud_out->points[i].normal_y = N[i].y;
      cloud_out->points[i].normal_z = N[i].z;
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZ> glm_to_pcl_vecXYZ(std::vector<glm::vec3> XYZ){
    pcl::PointCloud<pcl::PointXYZ> cloud;
    int size = XYZ.size();
    //---------------------------

    cloud.points.resize (size);
    for(int i=0; i<size; i++){
      cloud.points[i].x = XYZ[i].x;
      cloud.points[i].y = XYZ[i].y;
      cloud.points[i].z = XYZ[i].z;
    }

    //---------------------------
    return cloud;
  }
  pcl::PointCloud<pcl::PointXYZ>::Ptr glm_to_pcl_vecXYZ_ptr(std::vector<glm::vec3> XYZ){
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
    int size = XYZ.size();
    //---------------------------

    cloud->points.resize (size);
    for(int i=0; i<size; i++){
      cloud->points[i].x = XYZ[i].x;
      cloud->points[i].y = XYZ[i].y;
      cloud->points[i].z = XYZ[i].z;
    }

    //---------------------------
    return cloud;
  }

  pcl::PointCloud<pcl::PointXYZRGB>::Ptr glm_XYZRGB_to_pcl_XYZRGB(std::vector<glm::vec3> XYZ, std::vector<glm::vec4> RGB){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZRGB>);
    if(RGB.size() == 0) std::cout<<"No color data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud->points.resize (size);
    for(int i=0; i<size; i++){
      cloud->points[i].x = XYZ[i].x;
      cloud->points[i].y = XYZ[i].y;
      cloud->points[i].z = XYZ[i].z;

      cloud->points[i].r = (int)(RGB[i].x*255);
      cloud->points[i].g = (int)(RGB[i].y*255);
      cloud->points[i].b = (int)(RGB[i].z*255);
    }

    //---------------------------
    return cloud;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr glm_XYZRGBobj_to_pcl_XYZRGB(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZRGB>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    const std::vector<glm::vec4>& RGB = subset_in->RGB;
    if(RGB.size() == 0) std::cout<<"No color data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].r = (int)(RGB[i].x*255);
      cloud_out->points[i].g = (int)(RGB[i].y*255);
      cloud_out->points[i].b = (int)(RGB[i].z*255);
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr glm_XYZRGBinit_to_pcl_XYZRGB(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZRGB>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    const std::vector<glm::vec4>& RGB = subset_in->RGB;
    if(RGB.size() == 0) std::cout<<"No color data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].r = (int)(RGB[i].x*255);
      cloud_out->points[i].g = (int)(RGB[i].y*255);
      cloud_out->points[i].b = (int)(RGB[i].z*255);
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr glm_XYZIobj_to_pcl_XYZRGB(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZRGB>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    std::vector<float>& Is = subset_in->I;
    if(Is.size() == 0) std::cout<<"No intensity data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].r = (int)(Is[i]*255);
      cloud_out->points[i].g = (int)(Is[i]*255);
      cloud_out->points[i].b = (int)(Is[i]*255);
    }

    //---------------------------
    return cloud_out;
  }
  pcl::PointCloud<pcl::PointXYZRGB>::Ptr glm_XYZIinit_to_pcl_XYZRGB(Subset* subset_in){
    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZRGB>);
    const std::vector<glm::vec3>& XYZ = subset_in->xyz;
    std::vector<float>& Is = subset_in->I;
    if(Is.size() == 0) std::cout<<"No intensity data for conversion"<<std::endl;
    int size = XYZ.size();
    //---------------------------

    cloud_out->points.resize (size);
    for(int i=0; i<size; i++){
      cloud_out->points[i].x = XYZ[i].x;
      cloud_out->points[i].y = XYZ[i].y;
      cloud_out->points[i].z = XYZ[i].z;

      cloud_out->points[i].r = (int)(Is[i]*255);
      cloud_out->points[i].g = (int)(Is[i]*255);
      cloud_out->points[i].b = (int)(Is[i]*255);
    }

    //---------------------------
    return cloud_out;
  }

  //PCL to GLM
  Subset pcl_to_glm_XYZ(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in){
    Subset subset_out;
    std::vector<glm::vec3>& XYZ = subset_out.xyz;
    int size = cloud_in->width;
    //---------------------------

    glm::vec3 Point;
    for(int i=0; i<size; i++){
      Point.x = cloud_in->points[i].x;
      Point.y = cloud_in->points[i].y;
      Point.z = cloud_in->points[i].z;

      XYZ.push_back(Point);
    }

    //---------------------------
    return subset_out;
  }
  Subset pcl_to_glm_XYZI(pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_in){
    Subset subset_out;
    std::vector<glm::vec3>& XYZ = subset_out.xyz;
    std::vector<float>& Is = subset_out.I;
    int size = cloud_in->width;
    //---------------------------

    glm::vec3 Point;
    for(int i=0; i<size; i++){
      Point.x = cloud_in->points[i].x;
      Point.y = cloud_in->points[i].y;
      Point.z = cloud_in->points[i].z;

      XYZ.push_back(Point);
      Is.push_back(cloud_in->points[i].intensity);
    }

    //---------------------------
    return subset_out;
  }
  Subset pcl_to_glm_XYZRGB(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_in){
    Subset subset_out;
    std::vector<glm::vec3>& XYZ = subset_out.xyz;
    std::vector<glm::vec4>& RGB = subset_out.RGB;
    int size = cloud_in->width;
    //---------------------------

    glm::vec3 Point;
    glm::vec4 Color;
    for(int i=0; i<size; i++){
      Point.x = cloud_in->points[i].x;
      Point.y = cloud_in->points[i].y;
      Point.z = cloud_in->points[i].z;

      Color.x = (float)cloud_in->points[i].r/255.0f;
      Color.y = (float)cloud_in->points[i].g/255.0f;
      Color.z = (float)cloud_in->points[i].b/255.0f;
      Color.w = 1.0f;

      XYZ.push_back(Point);
      RGB.push_back(Color);
    }

    //---------------------------
    return subset_out;
  }

  std::vector<glm::vec3> pcl_XYZ_to_glm_vecXYZ(pcl::PointCloud <pcl::PointXYZ>::Ptr cloud){
    std::vector<glm::vec3> XYZ;
    glm::vec3 location;
    int size = cloud->size();
    //---------------------------

    for(int i=0; i<size; i++){
      bool a1 = isnan(cloud->points[i].x) == false;
      bool a2 = isnan(cloud->points[i].y) == false;
      bool a3 = isnan(cloud->points[i].z) == false;

      if(a1 && a2 && a3){
        location.x = cloud->points[i].x;
        location.y = cloud->points[i].y;
        location.z = cloud->points[i].z;

        XYZ.push_back(location);
      }
    }

    //---------------------------
    return XYZ;
  }
  std::vector<glm::vec3> pcl_XYZ_to_glm_vecXYZ(pcl::PointCloud <pcl::PointXYZ> cloud){
    std::vector<glm::vec3> XYZ;
    glm::vec3 location;
    int size = cloud.size();
    //---------------------------

    for(int i=0; i<size; i++){
      bool a1 = isnan(cloud.points[i].x) == false;
      bool a2 = isnan(cloud.points[i].y) == false;
      bool a3 = isnan(cloud.points[i].z) == false;

      if(a1 && a2 && a3){
        location.x = cloud.points[i].x;
        location.y = cloud.points[i].y;
        location.z = cloud.points[i].z;

        XYZ.push_back(location);
      }
    }

    //---------------------------
    return XYZ;
  }
  std::vector<glm::vec3> pcl_XYZRGB_to_glm_vecXYZ(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud){
    std::vector<glm::vec3> XYZ;
    glm::vec3 location;
    int size = cloud->points.size();
    //---------------------------

    for(int i=0; i<size; i++){
      bool a1 = isnan(cloud->points[i].x) == false;
      bool a2 = isnan(cloud->points[i].y) == false;
      bool a3 = isnan(cloud->points[i].z) == false;
      if(a1 && a2 && a3){
        location.x = cloud->points[i].x;
        location.y = cloud->points[i].y;
        location.z = cloud->points[i].z;

        XYZ.push_back(location);
      }
    }

    //---------------------------
    return XYZ;
  }
  std::vector<glm::vec4> pcl_XYZRGB_to_glm_vecRGB(pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud){
    std::vector<glm::vec4> Color;
    glm::vec4 RGB;
    int size = cloud->points.size();
    //---------------------------

    for(int i=0; i<size; i++){
      bool a1 = isnan(cloud->points[i].x) == false;
      bool a2 = isnan(cloud->points[i].y) == false;
      bool a3 = isnan(cloud->points[i].z) == false;
      if(a1 && a2 && a3){
        RGB.x = (float)cloud->points[i].r/255.0f;
        RGB.y = (float)cloud->points[i].g/255.0f;
        RGB.z = (float)cloud->points[i].b/255.0f;
        RGB.w = 1.0f;

        Color.push_back(RGB);
      }
    }

    //---------------------------
    return Color;
  }

}

#endif
#endif
