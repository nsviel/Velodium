#include "Operation.h"

#include "Functions/Selection.h"

#include "../Engine/Scene.h"
#include "../Engine/Glyphs.h"
#include "../Engine/Data/Database.h"
#include "../Load/Loader.h"

#include "../Specific/fct_transtypage.h"
#include "../Specific/fct_opengl.h"

#include <experimental/filesystem>
#include <set>

extern struct Database database;


//Constructor / destructor
Operation::Operation(){
  //---------------------------

  this->sceneManager = new Scene();
  this->loaderManager = new Loader();
  this->glyphManager = new Glyphs();

  this->spaceSampling = 0.08f;
  this->nbLineSampling = 1000000;

  //Get absolute executable location
  string absPath = std::experimental::filesystem::current_path();
  this->pathDir = absPath + '/' + configuration.INIT_DefaultDirPath;

  //---------------------------
}
Operation::~Operation(){}

//Specific functions
void Operation::fastScene(int mode){
  char path[PATH_MAX];
  //---------------------------

  switch(mode){
    case 0:{//Buddha
      sceneManager->removeCloud_all();

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
      sceneManager->removeCloud_all();
      loaderManager->load_cloud("../media/fastScene/torus_1.ply");
      loaderManager->load_cloud("../media/fastScene/torus_2.ply");
      break;
    }
    case 2:{//PCAP
      sceneManager->removeCloud_all();
      loaderManager->load_cloud("../media/fastScene/pcap_test.pcap");
      break;
    }
    case 3:{//Frames in movement
      sceneManager->removeCloud_all();
      this->loading_directoryFrames("../media/frames/");
      break;
    }
    case 4:{//more Frames in movement
      sceneManager->removeCloud_all();
      this->loading_directoryFrames("../media/frames_lot/");
      break;
    }
    case 5:{//Scala
      sceneManager->removeCloud_all();
      this->loading_directoryFrames("../media/frames_lot/");
      break;
    }
  }

  //---------------------------
}
void Operation::reset(){
  list<Cloud*>* list_Cloud = database.list_cloud;
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_Cloud->size(); i++){
    Cloud* cloud = *next(list_Cloud->begin(),i);
    sceneManager->update_cloud_reset(cloud);
  }

  //Reset all functions
  glyphManager->reset();
  sceneManager->update_cloud(cloud);

  //---------------------------
  console.AddLog("Reset scene...");
}

//Loading functions
void Operation::loading(){
  string zenity = "zenity --file-selection --multiple --title=Load --filename=" + pathDir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  //---------------------------

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    vector<string> CloudPaths;
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        CloudPaths.push_back(path_str.substr(0, path_str.find('|')));
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }
    CloudPaths.push_back(path_str);

    //Load files
    for(int i=0; i<CloudPaths.size(); i++){
      //Extract file path
      string path = CloudPaths[i];
      if (path_str.find('\n')){
        path.erase(std::remove(path.begin(), path.end(), '\n'), path.end()); //-> Supress unwanted line break
      }
      pathDir = path.substr(0, path.find_last_of("/")+1);

      //Load files
      loaderManager->load_cloud(path);
    }
  }

  //---------------------------
}
void Operation::loading_frames(){

  string zenity = "zenity --file-selection --multiple --title=Load --filename=" + pathDir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  //---------------------------

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Supress unwanted line break
    vector<string> CloudPaths;
    if (path_str.find('\n')){
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    //Check for multiple
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        CloudPaths.push_back(path_str.substr(0, path_str.find('|')));
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }
    CloudPaths.push_back(path_str);

    //Load files
    loaderManager->load_cloud_byFrame(CloudPaths);
  }

  //---------------------------
}
void Operation::loading_directoryFrames(string path){
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
void Operation::loading_sampling(){
  string zenity = "zenity --file-selection --multiple --title=Load --filename=" + pathDir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);
  //---------------------------

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    vector<string> CloudPaths;
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        CloudPaths.push_back(path_str.substr(0, path_str.find('|')));
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }
    CloudPaths.push_back(path_str);

    //Load files
    for(int i=0; i<CloudPaths.size(); i++){
      //Extract file path
      string path = CloudPaths[i];
      if (path_str.find('\n')){
        path.erase(std::remove(path.begin(), path.end(), '\n'), path.end()); //-> Supress unwanted line break
      }
      pathDir = path.substr(0, path.find_last_of("/")+1);

      //Load files
      int size = get_fileSize(path);
      if(size > 1000000){
        this->samplingLoader(path);
      }
    }
  }

  //---------------------------
}
void Operation::loading_treatment(){
  //---------------------------

  //Open Zenity window
  string zenity = "zenity --file-selection --multiple --title=Load --filename=" + pathDir + " 2> /dev/null";
  FILE *file = popen(zenity.c_str(), "r");
  char filename[32768];
  const char* path_char = fgets(filename, 32768, file);

  //Check if not empty
  if ((path_char != NULL) && (path_char[0] != '\0')){
    string path_str(path_char);

    //Check for multiple
    vector<string> CloudPaths;
    if (path_str.find('|')){
      int N = count(path_str.begin(), path_str.end(), '|');
      for(int i=0; i<N; i++){
        CloudPaths.push_back(path_str.substr(0, path_str.find('|')));
        path_str = path_str.substr(path_str.find('|')+1);
      }
    }
    CloudPaths.push_back(path_str);

    //Load files
    for(int i=0; i<CloudPaths.size(); i++){
      //Extract file path
      string path = CloudPaths[i];
      if (path_str.find('\n')){
        path.erase(std::remove(path.begin(), path.end(), '\n'), path.end()); //-> Supress unwanted line break
      }
      pathDir = path.substr(0, path.find_last_of("/")+1);

      //Load files
      loaderManager->load_cloud(path);
      Cloud* cloud = database.cloud_selected;
      loaderManager->save_cloud(cloud, path);
      sceneManager->removeCloud(cloud);
    }
  }

  //---------------------------
}
vector<string> Operation::get_directoryAllFilePath(string path){
  vector<string> allpath;
  //---------------------------

  for(const auto& entry : std::experimental::filesystem::directory_iterator(path)){
    allpath.push_back(entry.path());
  }

  //---------------------------
  return allpath;
}
void Operation::samplingLoader(string path){
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
    sceneManager->removeCloud(cloud_toAdd);

    //Space sampling the resulting cloud
    filterManager.spaceSampling(cloud_base, spaceSampling);
    cout<<" - "<<cloud_base->nb_point<<" points"<<endl;

    //Update final cloud
    sceneManager->update_cloud(cloud_base);
  }
*/
  //---------------------------
}
void Operation::selectDirectory(string& folderPath){
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
void Operation::saving(){
  //---------------------------

  //Open Zenity window
  string zenity = "zenity --file-selection --save --title=Save --filename=" + pathDir;
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);
    if (path_str.find('\n')){
      //Supress unwanted line break
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end());
    }

    Cloud* cloud = database.cloud_selected;
    loaderManager->save_cloud(cloud, path_str);
  }

  //---------------------------
}
void Operation::allSaving(){
  string zenity = "zenity --file-selection --directory --title=Save --filename=" + pathDir;
  //---------------------------

  //Retrieve dir path
  FILE *file = popen(zenity.c_str(), "r");
  char filename[1024];
  char* path_char = fgets(filename, 1024, file);

  //Check if empty
  if ((path_char != NULL) && (path_char[0] != '\0')) {
    string path_str(path_char);
    if (path_str.find('\n'))
      path_str.erase(std::remove(path_str.begin(), path_str.end(), '\n'), path_str.end()); //-> Supress unwanted line break

    //Save all cloud in the list
    for(int i=0; i<database.list_cloud->size(); i++){
      Cloud* cloud = *next(database.list_cloud->begin(),i);

      string pathFile = pathDir + "/" + cloud->name + ".pts";
      loaderManager->save_cloud(cloud, pathFile);
    }
  }

  //---------------------------
}
