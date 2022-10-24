#include "Saver.h"

#include "../Format/file_PTS.h"
#include "../Format/file_PLY.h"
#include "../Format/file_PTX.h"
#include "../Format/file_PCAP.h"
#include "../Format/file_CSV.h"
#include "../Format/file_OBJ.h"
#include "../Format/file_XYZ.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_system.h"
#include "../../Specific/fct_transtypage.h"


//Constructor / Destructor
Saver::Saver(Node_load* node_load){
  //---------------------------

  Node_engine* node_engine = node_load->get_node_engine();

  this->sceneManager = node_engine->get_sceneManager();

  this->ptsManager = new file_PTS();
  this->plyManager = new file_PLY();
  this->ptxManager = new file_PTX();
  this->csvManager = new file_CSV();
  this->objManager = new file_OBJ();
  this->xyzManager = new file_XYZ();
  this->pcapManager = new file_PCAP();

  //---------------------------
}
Saver::~Saver(){}

//Main function
bool Saver::save_cloud(Cloud* cloud, string filePath){
  string format = filePath.substr(filePath.find_last_of(".") + 1);
  bool success = false;
  //---------------------------

  //Check file format
  if(format.at(0) == '/' || format == "pts"){
    success = ptsManager->Exporter(filePath, cloud);
  }
  else if(format == "ply"){
    string format = "binary";
    success = plyManager->Exporter_cloud(filePath, format, cloud);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving point cloud");
    return false;
  }

  //---------------------------
  string log = "Saved " + filePath;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset(Subset* subset, string format, string dirPath){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, subset);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, subset);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving point cloud");
    return false;
  }

  //---------------------------
  string log = "Saved at " + dirPath;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset(Subset* subset, string format, string dirPath, string fileName){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, subset);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, subset, fileName);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving point cloud");
    return false;
  }

  //---------------------------
  string log = "Saved at " + dirPath;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset_silent(Subset* subset, string format, string dirPath){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, subset);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, subset);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving point cloud");
    return false;
  }

  //---------------------------
  return true;
}
