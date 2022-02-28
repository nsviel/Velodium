#include "Loader.h"

#include "Extractore.h"

#include "../Format/file_PTS.h"
#include "../Format/file_PLY.h"
#include "../Format/file_PTX.h"
#include "../Format/file_PCAP.h"
#include "../Format/file_CSV.h"
#include "../Format/file_OBJ.h"
#include "../Format/file_XYZ.h"

#include "../Load_node.h"

#include "../../Engine/Engine_node.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_system.h"
#include "../../Specific/fct_transtypage.h"


//Constructor / Destructor
Loader::Loader(Load_node* node_load){
  //---------------------------

  Engine_node* node_engine = node_load->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->extractManager = node_load->get_extractManager();

  this->ptsManager = new file_PTS();
  this->plyManager = new file_PLY();
  this->ptxManager = new file_PTX();
  this->csvManager = new file_CSV();
  this->objManager = new file_OBJ();
  this->xyzManager = new file_XYZ();
  this->pcapManager = new file_PCAP();

  //---------------------------
}
Loader::~Loader(){}

//Main functions
bool Loader::load_cloud(string filePath){
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    string log = "File doesn't exists: "+ filePath;
    console.AddLog("error", log);
    return false;
  }

  //Check file format & retrieve data
  vector<dataFile*> data_vec = load_retrieve_data(filePath);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  string log = "Loaded "+ filePath;
  console.AddLog("sucess", log);
  return true;
}
bool Loader::load_cloud_byFrame(vector<string> path_vec){
  vector<dataFile*> data_vec;
  //---------------------------

  //Retrieve data
  for(int i=0; i<path_vec.size(); i++){
    dataFile* data = plyManager->Loader(path_vec[i]);
    data_vec.push_back(data);
  }

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  string log = "Loaded " + to_string(data_vec.size()) + " frames";
  console.AddLog("sucess", log);
  return true;
}
bool Loader::load_cloud_silent(string filePath){
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    string log = "File doesn't exists: " + filePath;
    console.AddLog("error", log);
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
    console.AddLog("error", "File doesn't exists");
    return false;
  }

  //Check file format
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  if(format == "pts"){
    dataFile* data = ptsManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else{
    console.AddLog("error", "Failing loading point cloud");
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
    Subset* subset = sceneManager->get_subset(cloud_in, i);
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

//Sub-functions
vector<dataFile*> Loader::load_retrieve_data(string filePath){
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  vector<dataFile*> data_vec;
  //---------------------------

  if     (format == "pts"){
    dataFile* data = ptsManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "ptx"){
    dataFile* data = ptxManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "pcap"){
    data_vec = pcapManager->Loader(filePath);
  }
  else if(format == "pcd"){
    #ifdef FILE_PCD_H
    dataFile* data = pcdManager->Loader(filePath);
    data_vec.push_back(data);
    #endif
  }
  else if(format == "ply"){
    dataFile* data = plyManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "obj"){
    dataFile* data = objManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "xyz"){
    dataFile* data = xyzManager->Loader(filePath);
    data_vec.push_back(data);
  }
  else if(format == "csv"){
    data_vec = csvManager->Loader(filePath);
  }
  else{
    console.AddLog("error", "File format not recognized");
  }

  //---------------------------
  return data_vec;
}
void Loader::load_insertIntoDatabase(vector<dataFile*> data_vec){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Extract data and put in the engine
  cloud = extractManager->extractData(data_vec);
  list_cloud->push_back(cloud);

  //Update list cloud
  sceneManager->set_selected_cloud(cloud);
  sceneManager->update_cloud_oID(list_cloud);
  sceneManager->update_cloud_glyphs(cloud);

  //---------------------------
}
