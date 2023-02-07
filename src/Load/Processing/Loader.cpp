#include "Loader.h"

#include "Extractor.h"

#include "../Format/file_PTS.h"
#include "../Format/file_PLY.h"
#include "../Format/file_PTX.h"
#include "../Format/file_PCAP.h"
#include "../Format/file_CSV.h"
#include "../Format/file_OBJ.h"
#include "../Format/file_XYZ.h"
#include "../Format/file_LAS.h"
#include "../Format/file_CBOR.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Interface/File/Directory.h"
#include "../../Interface/File/Info.h"
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
  this->lasManager = new file_LAS();
  this->pcapManager = new file_PCAP();
  this->cborManager = new file_CBOR();

  //---------------------------
}
Loader::~Loader(){}

//Main functions
bool Loader::load_cloud(string path){
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    string log = "File doesn't exists: "+ path;
    console.AddLog("error", log);
    return false;
  }

  //Check file format & retrieve data
  vector<Data_file*> data_vec = load_retrieve_data(path);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  string log = "Loaded "+ path;
  console.AddLog("ok", log);
  return true;
}
bool Loader::load_cloud_byFrame(vector<string> path_vec){
  vector<Data_file*> data_vec;
  tic();
  //---------------------------

  //Retrieve data
  for(int i=0; i<path_vec.size(); i++){
    Data_file* data = plyManager->Loader(path_vec[i]);
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
  vector<Data_file*> data_vec;
  //---------------------------

  //Load only the first cloud
  Data_file* data = plyManager->Loader(path_vec[0]);
  data_vec.push_back(data);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //Save list of file
  cloud->list_path = path_vec;
  cloud->onthefly = true;
  cloud->ID_file++;

  //---------------------------
  string log = "Loaded on-the-fly cloud";
  console.AddLog("ok", log);
  return true;
}
bool Loader::load_cloud_silent(string path){
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    string log = "File doesn't exists: " + path;
    console.AddLog("error", log);
    return false;
  }

  //Check file format & retrieve data
  vector<Data_file*> data_vec = load_retrieve_data(path);

  //Extract data and put in the engine
  cloud = extractManager->extract_data(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_part(string path, int lmin, int lmax){
  vector<Data_file*> data_vec;
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    console.AddLog("error", "File doesn't exists");
    return false;
  }

  //Check file format
  string format = get_format_from_path(path);
  if(format == "pts"){
    Data_file* data = ptsManager->Loader(path);
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
  vector<Data_file*> data_vec;
  //---------------------------

  //Take input data
  for(int i=0; i<cloud_in->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud_in, i);
    Data_file* data = new Data_file();
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
  vector<Data_file*> data_vec;
  //---------------------------

  //Add NULL points
  Data_file* data = new Data_file();
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
bool Loader::load_cloud_oneFrame(Cloud* cloud, string path){
  //---------------------------

  if(is_file_exist(path)){
    //Retrieve data
    Data_file* data = plyManager->Loader(path);

    //Insert frame
    this->load_insertIntoCloud(data, cloud);

    //Delete raw data
    delete data;
  }else{
    return false;
  }

  //---------------------------
  return true;
}
vector<vec3> Loader::load_vertices(string path){
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    string log = "File doesn't exists: " + path;
    console.AddLog("error", log);
  }

  //Check file format & retrieve data
  vector<Data_file*> data_vec = load_retrieve_data(path);

  //Extract data
  vector<vec3> xyz = data_vec[0]->location;

  //---------------------------
  return xyz;
}

//Sub-functions
vector<Data_file*> Loader::load_retrieve_data(string path){
  string format = get_format_from_path(path);
  vector<Data_file*> data_vec;
  //---------------------------

  if     (format == "pts"){
    Data_file* data = ptsManager->Loader(path);
    data_vec.push_back(data);
  }
  else if(format == "ptx"){
    Data_file* data = ptxManager->Loader(path);
    data_vec.push_back(data);
  }
  else if(format == "pcap"){
    data_vec = pcapManager->Loader(path);
  }
  else if(format == "pcd"){
    #ifdef FILE_PCD_H
    Data_file* data = pcdManager->Loader(path);
    data_vec.push_back(data);
    #endif
  }
  else if(format == "ply"){
    Data_file* data = plyManager->Loader(path);
    data_vec.push_back(data);
  }
  else if(format == "obj"){
    Data_file* data = objManager->Loader(path);
    data_vec.push_back(data);
  }
  else if(format == "xyz"){
    Data_file* data = xyzManager->Loader(path);
    data_vec.push_back(data);
  }
  else if(format == "csv"){
    data_vec = csvManager->Loader(path);
  }
  else if(format == "cbor"){
    data_vec = cborManager->Loader(path);
  }
  else{
    console.AddLog("error", "File format not recognized");
  }

  //---------------------------
  return data_vec;
}
void Loader::load_insertIntoDatabase(vector<Data_file*> data_vec){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Extract data and put in the engine
  cloud = extractManager->extract_data(data_vec);
  list_cloud->push_back(cloud);

  //Update list cloud
  sceneManager->set_selected_cloud(cloud);
  sceneManager->update_cloud_oID(list_cloud);
  sceneManager->update_cloud_glyph(cloud);

  //Delete raw data
  for(int i=0; i<data_vec.size(); i++){
    delete data_vec[i];
  }

  //---------------------------
}
void Loader::load_insertIntoCloud(Data_file* data, Cloud* cloud){
  //---------------------------

  //Extract data and put in the engine
  extractManager->extract_data_frame(cloud, data);
  cloud->ID_file++;

  //---------------------------
}
