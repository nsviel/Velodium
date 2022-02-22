#include "Pather.h"

#include "Loader.h"
#include "Saver.h"

#include "../Engine/Scene.h"
#include "../Engine/Configuration/config_opengl.h"

#include "../Specific/fct_transtypage.h"
#include "../Specific/fct_system.h"

#include <Eigen/Dense>
#include <set>


//Constructor / destructor
Pather::Pather(){
  //---------------------------

  this->sceneManager = new Scene();
  this->loaderManager = new Loader();
  this->saverManager = new Saver();

  this->spaceSampling = 0.08f;
  this->nbLineSampling = 1000000;

  //Get absolute executable location
  config_opengl configManager;
  string path_init = configManager.parse_json_s("parameter", "path_media");
  this->path_current_dir = get_absolutePath_build() + '/' + path_init;

  //---------------------------
}
Pather::~Pather(){}

//Loading functions
void Pather::loading(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading();

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    loaderManager->load_cloud(path);
  }

  //---------------------------
}
void Pather::loading_frames(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading();

  //Load files
  if(path_vec.size() != 0){
    loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
}
void Pather::loading_directoryFrames(string path){
  //---------------------------

  //Supress unwanted line break
  if (path.find('\n')){
    path.erase(std::remove(path.begin(), path.end(), '\n'), path.end());
  }

  //Set order by name
  std::set<std::experimental::filesystem::path> sorted_by_name;
  for (auto &entry : std::experimental::filesystem::directory_iterator(path)){
   sorted_by_name.insert(entry.path());
  }

  //Get all frame path
  vector<string> path_vec;
  for(const auto& entry : sorted_by_name){
    string path_file = entry.c_str();

    if (path_file.find(".ply") != string::npos){
      path_vec.push_back(path_file);
    }
  }

  if(path_vec.size() != 0){
    loaderManager->load_cloud_byFrame(path_vec);
  }

  //---------------------------
}
void Pather::loading_sampling(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading();

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];
    int size = get_fileSize(path);
    if(size > 1000000){
      this->loading_sampledCloud(path);
    }
  }

  //---------------------------
}
void Pather::loading_sampledCloud(string path){
  /*  int nbLinePart = nbLineSampling;
  int nbPart = 0;
  int lmin = 0;
  int lmax = nbLinePart;
  int size = get_fileSize(path);
  Filter filterManager;
  //---------------------------

  //Load the "base" part
  loaderManager->load_cloud_part(path, lmin, lmax);
  Cloud* cloud_base = sceneManager->get_selectedcloud();
  Cloud* cloud_toAdd;

  //load and merge all other parts
  cout<<"Parameters : \n-> Nb lines per part : "<<nbLineSampling;
  cout<<"\n-> Space sampling : "<<spaceSampling<<endl;;
  while(lmax < size){
    cout<<"Loading part n°"<<nbPart;
    lmin = lmax;
    lmax = lmax + nbLinePart;
    nbPart++;

    //Load nth part
    loaderManager->load_cloud_part(path, lmin, lmax);
    cloud_toAdd = sceneManager->get_selectedcloud();

    //Merge the nth part with the base
    extractionManager->merging_addCloud(cloud_base, cloud_toAdd);

    //Supress nth added part
    sceneManager->remove_cloud(cloud_toAdd);

    //Space sampling the resulting cloud
    filterManager.spaceSampling(cloud_base, spaceSampling);
    cout<<" - "<<cloud_base->nb_point<<" points"<<endl;

    //Update final cloud
    sceneManager->update_cloud_glyphs(cloud_base);
  }
  */
  //---------------------------
}
void Pather::loading_treatment(){
  //---------------------------

  //select files
  vector<string> path_vec = this->zenity_loading();

  //Load files
  for(int i=0; i<path_vec.size(); i++){
    string path = path_vec[i];

    //Load files
    loaderManager->load_cloud(path);

    //Save and remove
    Cloud* cloud = sceneManager->get_cloud_selected();
    saverManager->save_cloud(cloud, path);
    sceneManager->remove_cloud(cloud);
  }

  //---------------------------
}
void Pather::loading_fastScene(int mode){
  char path[PATH_MAX];
  //---------------------------

  switch(mode){
    case 0:{//Buddha
      sceneManager->remove_cloud_all();

      Matrix4f realTransformation;
      realTransformation <<
        0.306093,   -0.951146,  -0.0403608,    0.585185,
        0.951862,      0.3065, -0.00415026 ,   0.289215,
        0.016318,  -0.0371476 ,   0.999177,   0.0189446,
        0,           0,           0,           1;

      loaderManager->load_cloud("../media/fastScene/buddha.pts");
      loaderManager->load_cloud("../media/fastScene/buddha_moved.pts");
      //loaderManager->load_cloud("../media/fastScene/buddha_moved.pts", realTransformation);
      break;
    }
    case 1:{//Torus
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("../media/fastScene/torus_1.ply");
      loaderManager->load_cloud("../media/fastScene/torus_2.ply");
      break;
    }
    case 2:{//PCAP
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("../media/fastScene/pcap_test.pcap");
      break;
    }
    case 3:{//Frames in movement
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("../media/point_cloud/frames/");
      break;
    }
    case 4:{//more Frames in movement
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("../media/point_cloud/frames_lot/");
      break;
    }
    case 5:{//Frame other
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("../media/point_cloud/frames_other/");
      break;
    }
    case 6:{//Frame other
      sceneManager->remove_cloud_all();
      loaderManager->load_cloud("/home/aether/Desktop/Point_cloud/HDL32-V2_Tunnel.pcap");
      break;
    }
    case 7:{//Frame for prediction
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("/home/aether/Desktop/Velodium/media/data/capture_test/frame/");
      break;
    }
    case 8:{//Frame for prediction
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("/home/aether/Desktop/Velodium/media/data/capture/test_statique");
      break;
    }
    case 9:{//Frame for prediction
      sceneManager->remove_cloud_all();
      this->loading_directoryFrames("/home/aether/Desktop/Velodium/media/data/capture/test_movement");
      break;
    }
  }

  //---------------------------
}

//Other functions
void Pather::saving(){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_saving();

  //Save current cloud
  Cloud* cloud = sceneManager->get_cloud_selected();
  saverManager->save_cloud(cloud, path_saving);

  //---------------------------
}
void Pather::saving_frameRange(int frame_b, int frame_e){
  //---------------------------

  //Select saving path
  string path_saving = this->zenity_saving();

  //Save current cloud
  Cloud* cloud = sceneManager->get_cloud_selected();
  for(int i=frame_b; i<frame_e; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    saverManager->save_subset(subset, ".ply", path_saving);
  }

  //---------------------------
}
void Pather::saving_allCloud(){
  //---------------------------

  //Select directory path
  string path_dir = this->zenity_directory();

  //Save all scene clouds
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    string pathFile = path_dir + "/" + cloud->name + ".pts";
    saverManager->save_cloud(cloud, pathFile);
  }

  //---------------------------
}

//Specific functions
vector<string> Pather::get_directoryAllFilePath(string path){
  vector<string> allpath;
  //---------------------------

  for(const auto& entry : std::experimental::filesystem::directory_iterator(path)){
    allpath.push_back(entry.path());
  }

  //---------------------------
  return allpath;
}
string Pather::get_filePath(){
  //---------------------------

  string path_str = "";

  //Zenity window
  string zenity = "zenity --file-selection 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    path_str = string(path_char);
  }

  //Check if there is a /n
  if(path_str.find('\n')){
    path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
  }

  //---------------------------
  return path_str;
}
string Pather::get_filePath(string path){
  string path_str;
  //---------------------------

  //Zenity window
  string zenity = "zenity --file-selection --filename=" + path + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    path_str = string(path_char);
  }
  else if(path_char == NULL){
    path_str = path;
  }

  //Check if there is a /n
  if(path_str.find('\n')){
    path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
  }

  //---------------------------
  return path_str;
}
void Pather::selectDirectory(string& folderPath){
  string zenity = "zenity --file-selection --directory --title=Directory --filename=" + folderPath + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  //---------------------------

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    //Change path dir
    folderPath = path_str;
  }

  //---------------------------
}

//Zenity stuff
vector<string> Pather::zenity_loading(){
  //---------------------------

  //Open zenity file manager
  string zenity = "zenity --file-selection --multiple --title=Load --filename=" + path_current_dir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  vector<string> path_vec;

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        string path_file = path_str.substr(0, path_str.find('|'));

        if (path_file.find('\n')){
          path_file.erase(std::remove(path_file.begin(), path_file.end(), '\n'), path_file.end());
        }

        path_vec.push_back(path_file);

        //Supress retrieved path from path list
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }

    //Retrieve the last selected path
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }
    path_vec.push_back(path_str);
  }

  //---------------------------
  return path_vec;
}
string Pather::zenity_saving(){
  string path_saving;
  //---------------------------

  //Open Zenity window
  string zenity = "zenity --file-selection --save --title=Save --filename=" + path_current_dir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    //Supress unwanted line break
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_saving = path_str;
  }

  //---------------------------
  return path_saving;
}
string Pather::zenity_directory(){
  string path_directory;
  //---------------------------

  //Retrieve dir path
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_current_dir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);

    //Remove unwanted break line
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    path_directory = path_str;
  }

  //---------------------------
  return path_directory;
}
void Pather::zenity_select_directory(string& path_dir){
  //---------------------------

  //Get absolute executable location
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + path_dir;

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

    path_dir = path_str + "/";
  }

  //---------------------------
}