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
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Info.h"
#include "../../Specific/Function/fct_transtypage.h"


//Constructor / Destructor
Saver::Saver(){
  //---------------------------

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
bool Saver::save_collection(Collection* collection, string path){
  string format = get_format_from_path(path);
  bool success = false;
  //---------------------------

  //Check file format
  if(format.at(0) == '/' || format == "pts"){
    success = ptsManager->Exporter(path, collection);
  }
  else if(format == "ply"){
    string format = "binary";
    success = plyManager->exporter_collection(collection, path, format);
    say(success);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving point collection");
    return false;
  }

  //---------------------------
  string log = "Saved " + path;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset(Cloud* cloud, string format, string dirPath){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, cloud);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, cloud);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving object");
    return false;
  }

  //---------------------------
  string log = "Saved at " + dirPath;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset(Cloud* cloud, string format, string dirPath, string fileName){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, cloud);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, cloud, fileName);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving object");
    return false;
  }

  //---------------------------
  string log = "Saved at " + dirPath;
  console.AddLog("ok", log);
  return true;
}
bool Saver::save_subset_silent(Cloud* cloud, string format, string dirPath){
  bool success = false;
  //---------------------------

  //If no format, add default ply
  if(format.at(0) == '/') format = "ply";

  //Check file format
  if     (format == "pts"){
    success = ptsManager->Exporter(dirPath, cloud);
  }
  else if(format == "ply"){
    string ply_format = "binary";
    success = plyManager->Exporter_subset(dirPath, ply_format, cloud);
  }

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving object");
    return false;
  }

  //---------------------------
  return true;
}
bool Saver::save_set_silent(Collection* collection, int ID, string path, int nb){
  bool success = false;
  //---------------------------

  success = plyManager->Exporter_set(path, "binary", collection, ID, nb);

  //Say if save is successfull
  if(!success){
    console.AddLog("error", "Failing saving collection");
    return false;
  }

  //---------------------------
  return true;
}
