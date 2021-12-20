#include "Capture.h"

#include "../../Engine/Scene.h"
#include "../../Load/Loader.h"
#include "../../Load/dataExtraction.h"
#include "../../Specific/timer.h"
#include "../../Specific/fct_display.h"

#include <experimental/filesystem>
#include <filesystem>


//Constructor / Destructor
Capture::Capture(){
  //---------------------------

  this->loaderManager = new Loader();
  this->extractManager = new dataExtraction();
  this->sceneManager = new Scene();

  this->ID_capture = 0;
  this->oneFrame = true;
  this->atLeastOne = false;
  this->path_dirSave = "/../media/data/";;

  //---------------------------
}
Capture::~Capture(){}

void Capture::check_forNewSubset(){
  //---------------------------

  if(atLeastOne){
    subset_capture.visibility = true;

    //Insert the subset inside the capture cloud
    if(oneFrame){
      //Check if at least one subset was created
      if(cloud_capture->subset.size() == 0){
        cloud_capture->subset.push_back(subset_capture);
        extractManager->add_subsetData(&subset_capture);
      }else{
        cloud_capture->subset[0].xyz = subset_capture.xyz;
        cloud_capture->subset[0].RGB = subset_capture.RGB;

        //Update subset data
        sceneManager->update_subset_location(&cloud_capture->subset[0]);
        sceneManager->update_subset_color(&cloud_capture->subset[0]);
        sceneManager->update_subset_glyphs(&cloud_capture->subset[0]);
      }
      cloud_capture->nb_subset = cloud_capture->subset.size();
    }
    else{
      //We cancel the visibility of the previous subset
      if(cloud_capture->subset.size() > 0){
        cloud_capture->subset[cloud_capture->subset.size()-1].visibility = false;
      }

      //We include the new one into the cloud
      cloud_capture->subset.push_back(subset_capture);
      extractManager->add_subsetData(&subset_capture);
      cloud_capture->nb_subset = cloud_capture->subset.size();
    }

  }

  //---------------------------
}

void Capture::new_capture(){
  //---------------------------

  //Reset variables
  this->ID_subset = 0;

  //Create new empty cloud
  loaderManager->load_cloud_empty();
  cloud_capture = loaderManager->get_createdcloud();
  cloud_capture->name = "Capture_" + to_string(ID_capture);

  //Special capture directory
  string absPath = std::experimental::filesystem::current_path();
  string dirPath = absPath + path_dirSave + cloud_capture->name;
  this->path_frameSave = absPath + path_dirSave + cloud_capture->name + "/";
  std::filesystem::create_directory(dirPath);

  //---------------------------
  ID_capture++;
}
void Capture::create_subset(udpPacket* frame_in, bool is_recording){
  //This function is made for being asynchronous !
  udpPacket frame = *frame_in;
  //---------------------------

  //Convert the udppacket into subset
  frame.name = "frame_" + to_string(ID_subset);
  subset_capture = extractManager->extractData(&frame);

  if(is_recording){
    loaderManager->save_subset(&subset_capture, "ply", path_frameSave);
  }

  this->atLeastOne = true;

  //---------------------------
  ID_subset++;
}
void Capture::recording_selectDirSave(){
    //---------------------------

    //Get absolute executable location
    string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_frameSave;

    //Retrieve dir path
    FILE *file = popen(zenity.c_str(), "r");
    char filename[1024];
    char* path_char = fgets(filename, 1024, file);

    //Check if empty
    if ((path_char != NULL) && (path_char[0] != '\0')) {
      string path_str(path_char);
      if (path_str.find('\n')){
        path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break
      }

      this->path_frameSave = path_str + "/";
    }

    //---------------------------
  }
