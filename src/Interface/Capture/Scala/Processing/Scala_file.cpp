#include "Scala_file.h"

#include "../../../../Specific/File/Zenity.h"
#include "../../../../Load/Node_load.h"
#include "../../../../Load/Processing/Loader.h"
#include "../../../../Engine/Node_engine.h"
#include "../../../../Scene/Node_scene.h"
#include "../../../../Scene/Data/Scene.h"

//1 frame = 2 lidar scans = 2046 points
//1024 points per scan with 25Hz rotation and 3 faisceau up and down
//1 timestamp per faisceau of the lidar for 340 points


//Constructor / Destructor
Scala_file::Scala_file(Node_engine* node_engine){
  //---------------------------

  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->loaderManager = node_load->get_loaderManager();

  //---------------------------
}
Scala_file::~Scala_file(){}

void Scala_file::loading(string pathDir){
  //---------------------------

  vector<string> allpath = loading_allPathDir(pathDir);
  vector<Collection*> clouds = loading_allFile(allpath);
  Collection* cloud_scala = loading_reoganizeData(clouds);
  this->compute_relativeTimestamp(cloud_scala);

  //---------------------------
}

vector<string> Scala_file::loading_allPathDir(string pathDir){
  //---------------------------

  vector<string> allpath = zenity_file_vec("Load Scala_file", pathDir);

  //---------------------------
  return allpath;
}
vector<Collection*> Scala_file::loading_allFile(vector<string> allpath){
  vector<Collection*> clouds;
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
      Collection* collection = (Collection*)loaderManager->get_created_collection();
      collection->path_file_load = allpath[i] + "/" + "scala" + ".csv";

      for(int j=0; j<collection->list_obj.size(); j++){
        Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), j);

        for(int k=0; k<cloud->rgb.size(); k++){
          cloud->rgb[k] = vec4(Red, Green, Blue, 1.0f);
        }

      }

      clouds.push_back(collection);
    }
  }

  //---------------------------
  return clouds;
}
Collection* Scala_file::loading_reoganizeData(vector<Collection*> clouds){
  Collection* cloud_scala = new Collection();
  cloud_scala->path_file_load = clouds[0]->path_file_load;
  Cloud* cloud;
  //---------------------------

  //ieme common cloud
  for(int i=0; i<clouds[0]->list_obj.size(); i++){

    //We accumulate 2 frame in one
    if(i == 0 || i % 2 == 0){
      cloud = new Cloud();
    }

    //jeme collection
    for(int j=0; j<clouds.size(); j++){
      Cloud* subset_scala = (Cloud*)clouds[j]->get_obj(i);

      //keme points
      for(int k=0; k<subset_scala->xyz.size(); k++){
        cloud->xyz.push_back(subset_scala->xyz[k]);
        cloud->rgb.push_back(subset_scala->rgb[k]);
        cloud->ts.push_back(subset_scala->ts[k]);
      }
    }

    if(i % 2 == 0){
      cloud_scala->list_obj.push_back(cloud);
    }
  }

  //Remove all old clouds
  for(int i=0; i<clouds.size(); i++){
    delete clouds[i];
  }

  //Load final collection
  loaderManager->load_cloud_creation(cloud_scala);
  Collection* collection = (Collection*)loaderManager->get_created_collection();

  //---------------------------
  return collection;
}
void Scala_file::compute_relativeTimestamp(Collection* collection){
  //---------------------------

  for(int i=0; i<1; i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    vector<float>& ts = cloud->ts;

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
