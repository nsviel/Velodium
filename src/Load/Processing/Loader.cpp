#include "Loader.h"

#include "Extractor.h"

#include "../Format/file_PTS.h"
#include "../Format/file_PLY.h"
#include "../Format/file_PTX.h"
#include "../Format/file_PCAP.h"
#include "../Format/file_CSV.h"
#include "../Format/file_OBJ.h"
#include "../Format/file_XYZ.h"
#include "../Format/file_CBOR.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_system.h"
#include "../../Specific/fct_transtypage.h"


//Constructor / Destructor
Loader::Loader(Node_load* node_load){
  //---------------------------

  Node_engine* node_engine = node_load->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();
  this->extractManager = node_load->get_extractManager();

  this->ptsManager = new file_PTS();
  this->plyManager = new file_PLY();
  this->ptxManager = new file_PTX();
  this->csvManager = new file_CSV();
  this->objManager = new file_OBJ();
  this->xyzManager = new file_XYZ();
  this->pcapManager = new file_PCAP();
  this->cborManager = new file_CBOR();

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
  console.AddLog("ok", log);
  return true;
}
bool Loader::load_cloud_byFrame(vector<string> path_vec){
  vector<dataFile*> data_vec;
  tic();
  //---------------------------

  //Retrieve data
  for(int i=0; i<path_vec.size(); i++){
    dataFile* data = plyManager->Loader(path_vec[i]);
    data_vec.push_back(data);
  }

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  int duration = (int)toc_ms();
  string log = "Loaded " + to_string(data_vec.size()) + " frames in " + to_string(duration) + " ms";
  console.AddLog("ok", log);
  return true;
}
bool Loader::load_cloud_onthefly(vector<string> path_vec){
  vector<dataFile*> data_vec;
  //---------------------------

  //Load only the first cloud
  dataFile* data = plyManager->Loader(path_vec[0]);
  data_vec.push_back(data);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //Save list of file
  cloud->list_path = path_vec;
  cloud->onthefly = true;
  cloud->ID_file++;

  //---------------------------
  int duration = (int)toc_ms();
  string log = "Loaded " + to_string(data_vec.size()) + " frames in " + to_string(duration) + " ms";
  console.AddLog("ok", log);
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
  cloud = extractManager->extract_data(data_vec);

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
bool Loader::load_cloud_oneFrame(Cloud* cloud){
  vector<string> list_path = cloud->list_path;
  int idx = cloud->ID_file;
  //---------------------------

  //Retrieve data
  dataFile* data = plyManager->Loader(list_path[idx]);

  //Insert frame
  this->load_insertIntoCloud(data, cloud);

  //---------------------------
  return true;
}
vector<vec3> Loader::load_vertices(string filePath){
  //---------------------------

  //Check file existence
  if(is_file_exist(filePath) == false){
    string log = "File doesn't exists: " + filePath;
    console.AddLog("error", log);
  }

  //Check file format & retrieve data
  vector<dataFile*> data_vec = load_retrieve_data(filePath);

  //Extract data
  vector<vec3> xyz = data_vec[0]->location;

  //---------------------------
  return xyz;
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
  else if(format == "cbor"){
    data_vec = cborManager->Loader(filePath);
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
  cloud = extractManager->extract_data(data_vec);
  list_cloud->push_back(cloud);

  //Update list cloud
  sceneManager->set_selected_cloud(cloud);
  sceneManager->update_cloud_oID(list_cloud);
  sceneManager->update_cloud_glyphs(cloud);

  //---------------------------
}
void Loader::load_insertIntoCloud(dataFile* data, Cloud* cloud){
  //---------------------------

  //Extract data and put in the engine
  extractManager->extract_data_frame(cloud, data);
  cloud->ID_file++;

  //---------------------------
}
