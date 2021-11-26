#include "Loader.h"

#include "dataExtraction.h"

#include "Format/file_PTS.h"
#include "Format/file_PLY.h"
#include "Format/file_PTX.h"
#include "Format/file_PCAP.h"
#include "Format/file_CSV.h"

#include "../Engine/Scene.h"
#include "../Engine/Data/Database.h"

#include "../Specific/fct_opengl.h"
#include "../Specific/fct_transtypage.h"

extern struct Database database;


//Constructor / Destructor
Loader::Loader(){
  //---------------------------

  this->extractManager = new dataExtraction();

  //---------------------------
}
Loader::~Loader(){}

//Loading function
bool Loader::load_cloud(string filePath){
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    console.AddLog("[error] File doesn't exists: %s", filePath.c_str());
    return false;
  }

  //Check file format & retrieve data
  vector<dataFile*> data_vec = load_retrieve_data(filePath);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  console.AddLog("[sucess] Loaded %s", filePath.c_str());
  return true;
}
bool Loader::load_cloud_byFrame(vector<string> path_vec){
  vector<dataFile*> data_vec;
  //---------------------------

  //Retrieve data
  filePLY plyManager;
  for(int i=0; i<path_vec.size(); i++){
    dataFile* data = plyManager.Loader(path_vec[i]);
    data_vec.push_back(data);
  }

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  console.AddLog("[sucess] Loaded all frames");
  return true;
}
bool Loader::load_cloud_silent(string filePath){
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    console.AddLog("[error] File doesn't exists: %s", filePath.c_str());
    return false;
  }

  //Check file format & retrieve data
  vector<dataFile*> data_vec = load_retrieve_data(filePath);

  //Extract data and put in the engine
  cloud = extractManager->extractData(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_part(string filePath, int lmin, int lmax){
  vector<dataFile*> data_vec;
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    console.AddLog("[error] File doesn't exists");
    return false;
  }

  //Check file format
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  if(format == "pts"){
    filePTS ptsManager;
    dataFile* data = ptsManager.Loader(filePath);
    data_vec.push_back(data);
  }
  else{
    console.AddLog("[error] Failing loading point cloud");
    return false;
  }

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_creation(Cloud* cloud_in){
  vector<dataFile*> data_vec;
  //---------------------------

  //Take input data
  for(int i=0; i<cloud_in->subset.size(); i++){
    Subset* subset = &cloud_in->subset[i];
    dataFile* data = new dataFile();
    data->path = cloud_in->path;

    //Location
    if(subset->xyz.size() != 0){
      data->location = subset->xyz;
    }

    //Color
    if(subset->RGB.size() != 0){
      data->color = subset->RGB;
    }

    //Intensity
    if(subset->I.size() != 0){
      data->intensity = subset->I;
    }

    //Timestamp
    if(subset->ts.size() != 0){
      data->timestamp = subset->ts;
    }

    //Normal
    if(subset->N.size() != 0){
      data->normal = subset->N;
    }

    data_vec.push_back(data);
  }

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_empty(){
  vector<dataFile*> data_vec;
  //---------------------------

  //Add NULL points
  dataFile* data = new dataFile();
  data->path = "../media/frame.ply";

  data->location.push_back(vec3(0.0f,0.0f,0.0f));
  data->location.push_back(vec3(1.0f,1.0f,1.0f));
  data->location.push_back(vec3(0.5f,0.5f,0.5f));

  data->color.push_back(vec4(0.0f,0.0f,0.0f,1.0f));
  data->color.push_back(vec4(0.0f,0.0f,0.0f,1.0f));
  data->color.push_back(vec4(0.0f,0.0f,0.0f,1.0f));

  data_vec.push_back(data);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_oneFrame(){
  string filePath = "frame.pts";
  dataFile* data = new dataFile();
  //---------------------------

  //Add an unique NULL point
  vec3 zero = vec3(0, 0, 0);
  data->location.push_back(zero);

  //Extraction of data
  data->path = filePath;
  vector<dataFile*> data_vec;
  data_vec.push_back(data);
  cloud = extractManager->extractData(data_vec);

  //---------------------------
  return true;
}

vector<dataFile*> Loader::load_retrieve_data(string filePath){
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  vector<dataFile*> data_vec;
  //---------------------------

  if     (format == "pts"){
    filePTS ptsManager;
    dataFile* data = ptsManager.Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "ptx"){
    filePTX ptxManager;
    dataFile* data = ptxManager.Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "pcap"){
    filePCAP pcapManager;
    data_vec = pcapManager.Loader(filePath);
  }
  else if(format == "pcd"){
    #ifdef FILE_PCD_H
    filePCD pcdManager;
    dataFile* data = pcdManager.Loader(filePath);
    data_vec.push_back(data);
    #endif
  }
  else if(format == "ply"){
    filePLY plyManager;
    dataFile* data = plyManager.Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "obj"){
    dataFile* data = OBJLoader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "xyz"){
    dataFile* data = XYZLoader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "csv"){
    fileCSV csvManager;
    data_vec = csvManager.Loader(filePath);
  }
  else{
    console.AddLog("[error] File format not recognized");
  }

  //---------------------------
  return data_vec;
}
void Loader::load_insertIntoDatabase(vector<dataFile*> data_vec){
  //---------------------------

  //Extract data and put in the engine
  cloud = extractManager->extractData(data_vec);
  database.list_cloud->push_back(cloud);
  database.cloud_selected = cloud;

  //Update list cloud
  Scene sceneManager;
  sceneManager.update_cloud_oID(database.list_cloud);
  sceneManager.update_cloud(cloud);

  //---------------------------
}

//Saving function
bool Loader::save_cloud(Cloud* cloud, string filePath){
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  bool sucess = false;
  //---------------------------

  //Check file format
  if(format.at(0) == '/') format = "pts";

  if     (format == "pts"){
    filePTS ptsManager;
    sucess = ptsManager.Exporter(filePath, cloud);
  }
  else if(format == "ply"){
    string format = "binary";
    filePLY plyManager;
    sucess = plyManager.Exporter_cloud(filePath, format, cloud);
  }

  //Say if save is successfull
  if(!sucess){
    console.AddLog("[error] Failing saving point cloud");
    return false;
  }

  //---------------------------
  console.AddLog("[sucess] Saved %s", filePath.c_str());
  return true;
}
bool Loader::save_subset(Subset* subset, string format, string dirPath){
  bool sucess = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    filePTS ptsManager;
    sucess = ptsManager.Exporter(dirPath, cloud);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    filePLY plyManager;
    sucess = plyManager.Exporter_subset(dirPath, ply_format, subset);
  }

  //Say if save is successfull
  if(!sucess){
    console.AddLog("[error] Failing saving point cloud");
    return false;
  }

  //---------------------------
  console.AddLog("[sucess] Saved at %s", dirPath.c_str());
  return true;
}
bool Loader::save_subset(Subset* subset, string format, string dirPath, string fileName){
  bool sucess = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    filePTS ptsManager;
    sucess = ptsManager.Exporter(dirPath, cloud);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    filePLY plyManager;
    sucess = plyManager.Exporter_subset(dirPath, ply_format, subset, fileName);
  }

  //Say if save is successfull
  if(!sucess){
    console.AddLog("[error] Failing saving point cloud");
    return false;
  }

  //---------------------------
  console.AddLog("[sucess] Saved at %s", dirPath.c_str());
  return true;
}

//Specific formats
dataFile* Loader::OBJLoader(string filePath){
  std::ifstream infile(filePath);
  std::string line;
  float a, b, c;
  string ID;
  dataFile* data = new dataFile();
  //---------------------------

  float R = float(rand()%101)/100;
  float G = float(rand()%101)/100;
  float B = float(rand()%101)/100;

  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> ID >> a >> b >> c;

    //Data extraction
    if(ID == "v" ) data->location.push_back(vec3(a, b, c));
    if(ID == "vn") data->normal.push_back(vec3(a, b, c));
  }

  //---------------------------
  return data;
}
dataFile* Loader::XYZLoader(string filePath){
  dataFile* data = new dataFile();
  //---------------------------

  //Open file
  std::ifstream infile(filePath);

  //Retrieve data
  std::string line;
  float a, b, c, d, e, f;
  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> a >> b >> c >> d >> e >> f;

    //Data extraction
    data->location.push_back(vec3(a, b, c));
    data->color.push_back(vec4(d, e, f, 1));
  }

  //---------------------------
  return data;
}
