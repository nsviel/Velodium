---
title: 'Lidium: a 3D point cloud processing software for TLS LiDAR data'
tags:
  - LiDAR
  - Point cloud
  - Intensity correction and calibration
authors:
  - name: Nathan Sanchiz-Viel^[corresponding author]
    orcid: 0000-0002-4608-5850
    affiliation: 1
affiliations:
 - name: Laboratoire Modélisation, Information et Systèmes (MIS), Université de Picardie Jules Verne (UPJV), 14 quai de la Somme, 80000 Amiens, France
   index: 1
date: 8 June 2021
bibliography: paper.bib
---

# Summary

LiDAR (Light Detection and Ranging) sensors give huge amount of data, which generally need a variety of post-processing actions. These data are 3D point cloud containing the spatial coordinates and additional information like color or intensity of a numerous set of measures. Generally, the main use of these point clouds is the construction of a numerical 3D model of a real scene. The registration of point clouds took from different points of view, for example, is a painful task to achieve and requires usually some specific steps : cloud sampling and filtering, normal computation, just to name a few. But point clouds could also be particularly useful with the support of the color or intensity information for object detection, material and structure recognition. A radiometric correction of the intensity is necessary to take advantage of this by-product information. Thus, we propose in this software an open source solution for visualization, easy manipulation and processing of point clouds, and some advanced algorithms for 1) radiometric correction of the the intensity information, 2) pairwise registration of multiple point cloud using different strategies.

![Point cloud of a sculpture of Niki de Saint Phalle, Angers, France.](image/figure.png)


# Statement of need

`Lidium` is designed as a laboratory software to be used by both LiDAR researchers and by geomatic students for the manipulation and algorithm prototyping applied to point cloud data. This software is distinguished of the other open source solutions as CloudCompare (http://www.cloudcompare.org/) or Meshlab (https://www.meshlab.net/) by providing a lightweight and hackable framework to LiDAR data visualization and processing, with a focus on the point cloud registration step and the radiometric correction of the intensity information. This software aim to make 3D digitalization a way more accessible, as an alternative solution from proprietary software.

The main difficulty of the three-dimensional reconstruction from the acquired point clouds, is the registration step. This one consists in the identification of the proper geometrical transformations allowing to regroup the point clouds in a common coordinate system. To do this, it is necessary to identify correspondences between the common areas of the clouds. This difficult problem concentrates the efforts of the research community. We use here an additional information acquired by the LiDAR, the intensity, as a discriminant element. This information is, by nature, insensible at external illuminations and related to the reflectance of the scanned material. However, the intensity is not widely used practically. Some effects induced by measure geometrical parameters and internal scanner treatments, make it strongly dependent to the viewpoint of the measure [@Kashani:2015]. 

We propose several approaches of radiometric correction and calibration, which under certain conditions, allow to make the intensity independent from the viewpoint and to convert it in a linear scale. A corrected intensity could also be useful in a large field of application : autonomous car for obstacle detection, clustering/segmentation, material or structure detection and recognition. 'Lidium' incorporate some research algorithms to proceed with the radiometric correction and calibration of this intensity. However, the intensity is scanner-dependent and the dataset used for the correction have to be made with the LiDAR of the user to get advantage of the present algorithms. The methodology and some algorithms are described in [@sanchiz:2021].

The use of this corrected or calibrated intensity in a registration pipeline show an increasing number of identified correspondences between two point clouds with the corrected or calibrated intensity. The corrected intensity, used with keypoint algorithms (SIFT, SHOT, RANSAC) give significant results to resolve this problematic stage. An implementation of an ICP based registration pipeline is provided with a separated matching step. These algorithms are fully adjustable to test parameters and methods adjustment.

All the code is made in C++, for performance purpose. The point cloud visualization is made with OpenGL and the data graphic generater is made with Gnuplot [@Gnuplot:2013]. Also, some external librairies as PCL [@Pcl:2011], FLANN [@flann:2009] and Eigen [@Eigen:2010] are diversely used for specific treatments.


# Acknowledgements

This work was supported by the Agence National de la Recherche (ANR) french institut, Paris, inside the SUMUM project, which aims to improve process and tools for heritage digitization. The authors declare no conflicts of interest.


# References
