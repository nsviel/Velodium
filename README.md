# Velodium
**Open Source 3D Point Cloud Processing Software**

## Summary

LiDAR (Light Detection and Ranging) sensors give huge amount of data, which generally need a variety of post-processing actions. These data are 3D point cloud containing the spatial coordinates and additional information like color or intensity of a numerous set of measures. Generally, the main use of these point clouds is the construction of a numerical 3D model of a real scene. This software proposes an alternative point cloud processing framework which permits to work on static and dynamic point clouds from different formats.

## Characteristics

<details><summary>Supported file formats</summary>

Import: pts, ply, ptx, obj, csv

Export: pts, ply

</details>

<details><summary>Dependencies</summary>

All of the code is written in C++, for performance purpose. The incorporated external libraries come from the open source world:
- OpenGL (https://www.opengl.org): Data visualization
- ImGui (https://github.com/ocornut/imgui): Graphical user interface
- PCL (https://pointclouds.org/): a set of processing algorithms
- Eigen (https://eigen.tuxfamily.org/): Some linear algebra
- FLANN (https://github.com/flann-lib/flann) & Nanoflann (https://github.com/jlblancoc/nanoflann) : Nearest neighbor search
- Gnuplot (http://www.gnuplot.info/): Graphical data visualization
- OptimLib (https://www.kthohr.com/optimlib.html): Numerical optimization methods
- normals_Hough (https://github.com/aboulch/normals_Hough): Method for normal estimation
- OpenMP (https://www.openmp.org/): Multithreading
- FontAwesome (https://fontawesome.com/): Icons

</details>

## Documentation

<details><summary>Installation</summary>
This code is only supported on Linux OS.
Tested on Ubuntu 18.04LTS, 20.04LTS, 22.04LTS.

Simply run the script file
```
cd /Lidium
./install.sh
```

Compile the executable:
```
 mkdir build && cd build && cmake .. && make -j4
```

</details>
<details><summary>Execution</summary>

To start the program run:
```
./executable
```

Some arguments could be added:
```
- capture                 [LiDAR capture mode]
- ai                      [AI interface mode]
- server                  [Server mode]
- load + absolute_path    [Load a cloud at start]

```

</details>
