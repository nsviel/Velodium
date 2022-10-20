#include "Scala_file.h"

#include "../../../../../Specific/fct_zenity.h"
#include "../../../../../Load/Node_load.h"
#include "../../../../../Load/Processing/Loader.h"
#include "../../../../../Engine/Node_engine.h"
#include "../../../../../Engine/Scene/Scene.h"

//1 frame = 2 lidar scans = 2046 points
//1024 points per scan with 25Hz rotation and 3 faisceau up and down
//1 timestamp per faisceau of the lidar for 340 points


//Constructor / Destructor
Scala_file::Scala_file(Node_engine* node_engine){
  //---------------------------

  Node_load* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->loaderManager = node_load->get_loadManager();

  //---------------------------
}
Scala_file::~Scala_file(){}

void Scala_file::loading(string pathDir){
  //---------------------------

  vector<string> allpath = loading_allPathDir(pathDir);
  vector<Cloud*> clouds = loading_allFile(allpath);
  Cloud* cloud_scala = loading_reoganizeData(clouds);
  this->compute_relativeTimestamp(cloud_scala);

  //---------------------------
}

vector<string> Scala_file::loading_allPathDir(string pathDir){
  //---------------------------

  vector<string> allpath = zenity_file_vec("Load Scala_file", pathDir);

  //---------------------------
  return allpath;
}
vector<Cloud*> Scala_file::loading_allFile(vector<string> allpath){
  vector<Cloud*> clouds;
  //---------------------------

  string format = "csv";

  for(int i=0; i<allpath.size(); i++){
    string path = allpath[i];
    string format = path.substr(path.find_last_of(".") + 1);

    float Red = float(rand()%101)/100;
    float Green = float(rand()%101)/100;
    float Blue = float(rand()%101)/100;

    if(format == "csv"){
      loaderManager->load_cloud_silent(path);
      Cloud* cloud = loaderManager->get_createdcloud();
      cloud->path = allpath[i] + "/" + "scala" + ".csv";

      for(int j=0; j<cloud->subset.size(); j++){
        Subset* subset = *next(cloud->subset.begin(), j);

        for(int k=0; k<subset->RGB.size(); k++){
          subset->RGB[k] = vec4(Red, Green, Blue, 1.0f);
        }

      }

      clouds.push_back(cloud);
    }
  }

  //---------------------------
  return clouds;
}
Cloud* Scala_file::loading_reoganizeData(vector<Cloud*> clouds){
  Cloud* cloud_scala = new Cloud();
  cloud_scala->path = clouds[0]->path;
  Subset* subset;
  //---------------------------

  //ieme common subset
  for(int i=0; i<clouds[0]->subset.size(); i++){

    //We accumulate 2 frame in one
    if(i == 0 || i % 2 == 0){
      subset = new Subset();
    }

    //jeme cloud
    for(int j=0; j<clouds.size(); j++){
      Subset* subset_scala = sceneManager->get_subset(clouds[j], i);

      //keme points
      for(int k=0; k<subset_scala->xyz.size(); k++){
        subset->xyz.push_back(subset_scala->xyz[k]);
        subset->RGB.push_back(subset_scala->RGB[k]);
        subset->ts.push_back(subset_scala->ts[k]);
      }
    }

    if(i % 2 == 0){
      cloud_scala->subset.push_back(subset);
    }
  }

  //Remove all old clouds
  for(int i=0; i<clouds.size(); i++){
    delete clouds[i];
  }

  //Load final cloud
  loaderManager->load_cloud_creation(cloud_scala);
  Cloud* cloud = loaderManager->get_createdcloud();

  //---------------------------
  return cloud;
}
void Scala_file::compute_relativeTimestamp(Cloud* cloud){
  //---------------------------

  for(int i=0; i<1; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    vector<float>& ts = subset->ts;

    float ts_cpt = ts[0];
    for(int j=0; j<ts.size(); j++){

      //If not
      if(ts[j] != ts_cpt){
        ts_cpt = ts[j];
      }

      //If the timestamp is the same as before : increment
      if(ts[j] == ts_cpt){
        float delay = (1 / (25 * 340)) * 1000000;
        ts[j] = ts_cpt + j * delay;
      }
    }

  }

  //---------------------------
}
