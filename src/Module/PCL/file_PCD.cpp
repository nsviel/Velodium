#ifdef WITH_PCL
#include "file_PCD.h"


//Constructor / Destructor
filePCD::filePCD(){}
filePCD::~filePCD(){}

//Main function
Data_file* filePCD::Loader(string pathFile){
  data_out = new Data_file();
  data_out->path = pathFile;
  //---------------------------

  //HEADER
  string header = Loader_header(pathFile);

  if(header == "XYZ"){
    this->Loader_XYZ(pathFile);
  }else if(header == "XYZI"){
    this->Loader_XYZI(pathFile);
  }

  //---------------------------
  return data_out;
}

//Subfunctions
string filePCD::Loader_header(string pathFile){
  ifstream infile1(pathFile);
  string a,b,c,d,e,f,g,h;
  string line;
  //---------------------------

  for(int i=0; i<10; i++){
    getline(infile1, line);
    istringstream iss(line);
    iss >> a >> b >> c >> d >> e >> f >> g >> h;
    if(a == "FIELDS"){
      if(e == "intensity"){
        return "XYZI";
      }
    }
  }

  //---------------------------
  return "XYZ";
}
void filePCD::Loader_XYZ(string pathFile){
  //---------------------------

  //Read file
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  bool readOK = pcl::io::loadPCDFile<pcl::PointXYZ> (pathFile, *cloud);

  //Check
  if(readOK == -1){
    PCL_ERROR("Couldn't read file test_pcd.pcd \n");
    return;
  }

  //Convert
  vec3 Point;
  int size = cloud->width;
  for(int i=0; i<size; i++){
    Point.x = cloud->points[i].x;
    Point.y = cloud->points[i].y;
    Point.z = cloud->points[i].z;

    data_out->location.push_back(Point);
  }

  //---------------------------
  data_out->size = size;
}
void filePCD::Loader_XYZI(string pathFile){
  //---------------------------

  //Read file
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>);
  bool readOK = pcl::io::loadPCDFile<pcl::PointXYZI> (pathFile, *cloud);

  //Check
  if(readOK == -1){
    PCL_ERROR("Couldn't read file test_pcd.pcd \n");
    return;
  }

  //Convert
  vec3 Point;
  int size = cloud->width;
  for(int i=0; i<size; i++){
    Point.x = cloud->points[i].x;
    Point.y = cloud->points[i].y;
    Point.z = cloud->points[i].z;

    data_out->location.push_back(Point);
    data_out->intensity.push_back(cloud->points[i].intensity/255);
  }

  //---------------------------
  data_out->size = size;
}

#endif
