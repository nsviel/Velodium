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

#include "../../Scene/Data/Scene.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Info.h"
#include "../../Specific/Function/fct_transtypage.h"


//Constructor / Destructor
Loader::Loader(){
  //---------------------------

  this->sceneManager = new Scene();
  this->extractManager = new Extractor();
  this->data = Data::get_instance();

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
Loader::~Loader(){
  //---------------------------

  delete ptsManager;
  delete plyManager;
  delete ptxManager;
  delete csvManager;
  delete objManager;
  delete xyzManager;
  delete lasManager;
  delete pcapManager;
  delete cborManager;

  //---------------------------
}

//Main functions
Collection* Loader::load_cloud(string path){
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    string log = "File doesn't exists: "+ path;
    console.AddLog("error", log);
  }

  //Check file format & retrieve data
  vector<Data_file*> data_vec = load_retrieve_cloud_data(path);

  //Insert cloud
  Collection* collection = load_insertIntoDatabase(data_vec);

  //---------------------------
  string log = "Loaded "+ path;
  console.AddLog("ok", log);
  return collection;
}
Object_* Loader::load_object(string path){
  //---------------------------

  //Check file existence
  if(is_file_exist(path) == false){
    string log = "File doesn't exists: "+ path;
    console.AddLog("error", log);
  }

  //Check file format & retrieve data
  Data_file* data_file = load_retrieve_data(path);

  //Insert cloud
  Object_* object = load_insertIntoDatabase(data_file);

  //---------------------------
  string log = "Loaded "+ path;
  console.AddLog("ok", log);
  return object;
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
  collection->list_otf_path = path_vec;
  collection->is_onthefly = true;
  collection->ID_obj_otf++;

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
  vector<Data_file*> data_vec = load_retrieve_cloud_data(path);

  //Extract data and put in the engine
  this->collection = extractManager->extract_data(data_vec);

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
bool Loader::load_cloud_creation(Collection* cloud_in){
  vector<Data_file*> data_vec;
  //---------------------------

  //Take input data
  for(int i=0; i<cloud_in->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)cloud_in->get_obj(i);
    Data_file* data = new Data_file();
    data->path_file = cloud_in->path_file_load;

    //Location
    if(cloud->xyz.size() != 0){
      data->xyz = cloud->xyz;
    }

    //Color
    if(cloud->rgb.size() != 0){
      data->rgb = cloud->rgb;
    }

    //Intensity
    if(cloud->I.size() != 0){
      data->I = cloud->I;
    }

    //Timestamp
    if(cloud->ts.size() != 0){
      data->ts = cloud->ts;
    }

    //Normal
    if(cloud->Nxyz.size() != 0){
      data->Nxyz = cloud->Nxyz;
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
  data->path_file = "../media/frame.ply";

  data->xyz.push_back(vec3(0.0f,0.0f,0.0f));
  data->xyz.push_back(vec3(1.0f,1.0f,1.0f));
  data->xyz.push_back(vec3(0.5f,0.5f,0.5f));

  data->rgb.push_back(vec4(0.0f,0.0f,0.0f,1.0f));
  data->rgb.push_back(vec4(0.0f,0.0f,0.0f,1.0f));
  data->rgb.push_back(vec4(0.0f,0.0f,0.0f,1.0f));

  data_vec.push_back(data);

  //Insert cloud
  this->load_insertIntoDatabase(data_vec);

  //---------------------------
  return true;
}
bool Loader::load_cloud_oneFrame(Collection* collection, string path){
  //---------------------------

  if(is_file_exist(path)){
    //Retrieve data
    Data_file* data = plyManager->Loader(path);

    //Insert frame
    this->load_insertIntoCloud(data, collection);

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
  vector<Data_file*> data_vec = load_retrieve_cloud_data(path);

  //Extract data
  vector<vec3> xyz = data_vec[0]->xyz;

  //---------------------------
  return xyz;
}

//Sub-functions
Data_file* Loader::load_retrieve_data(string path){
  string format = get_format_from_path(path);
  Data_file* data_out;
  //---------------------------

  if     (format == "pts"){
    data_out = ptsManager->Loader(path);
  }
  else if(format == "ptx"){
    data_out = ptxManager->Loader(path);
  }
  else if(format == "pcd"){
    #ifdef FILE_PCD_H
    data_out = pcdManager->Loader(path);
    #endif
  }
  else if(format == "ply"){
    data_out = plyManager->Loader(path);
  }
  else if(format == "obj"){
    data_out = objManager->Loader(path);
  }
  else if(format == "xyz"){
    data_out = xyzManager->Loader(path);
  }
  else{
    console.AddLog("error", "File format not recognized");
  }

  //---------------------------
  return data_out;
}
vector<Data_file*> Loader::load_retrieve_cloud_data(string path){
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
Collection* Loader::load_insertIntoDatabase(vector<Data_file*> data_vec){
  list<Collection*>* list_collection = data->get_list_collection();
  //---------------------------

  //Extract data and put in the engine
  this->collection = extractManager->extract_data(data_vec);
  list_collection->push_back(collection);

  //Update list collection
//  cloud_selected = collection;
  data->set_selected_collection(collection);
  data->update_ID_order();
  ////sceneManager->update_glyph(collection);

  //Delete raw data
  for(int i=0; i<data_vec.size(); i++){
    delete data_vec[i];
  }

  //---------------------------
  return collection;
}
Object_* Loader::load_insertIntoDatabase(Data_file* data_file){
  //---------------------------

  //Extract data and put in the engine
  Object_* object = extractManager->extract_data_object(data_file);

  //Update list cloud
  //data->set_selected_collection(object);
  //data->update_ID_order();
  ////sceneManager->update_glyph(collection);

  //Delete raw data
  delete data_file;

  //---------------------------
  return object;
}
void Loader::load_insertIntoCloud(Data_file* data, Collection* collection){
  //---------------------------

  //Extract data and put in the engine
  extractManager->extract_data(collection, data);
  collection->ID_obj_otf++;

  //---------------------------
}
