#include "Scala.h"

#include "../../Operation/Operation.h"
#include "../../Load/Loader.h"
#include "../../Engine/Scene.h"


//Constructor / Destructor
Scala::Scala(){
  //---------------------------

  //---------------------------
}
Scala::~Scala(){}

void Scala::loading(){
  vector<Cloud*> clouds;
  //---------------------------

  string pathDir = "";
  string format = "csv";

  Operation opeManager;
  opeManager.selectDirectory(pathDir);
  vector<string> allpath = opeManager.get_directoryAllFilePath(pathDir);

  Loader loaderManager;
  for(int i=0; i<allpath.size(); i++){
    string path = allpath[i];
    string format = path.substr(path.find_last_of(".") + 1);

    float Red = float(rand()%101)/100;
    float Green = float(rand()%101)/100;
    float Blue = float(rand()%101)/100;

    if(format == "csv"){
      loaderManager.load_cloud_silent(path);
      Cloud* cloud = loaderManager.get_createdcloud();
      cloud->path = pathDir + "/" + "scala" + ".csv";

      for(int j=0; j<cloud->subset.size(); j++){
        for(int k=0; k<cloud->subset[j].RGB.size(); k++){
          cloud->subset[j].RGB[k] = vec4(Red, Green, Blue, 1.0f);
        }
      }

      clouds.push_back(cloud);
    }
  }

  this->reorganize_data(clouds);

  //---------------------------
}

void Scala::reorganize_data(vector<Cloud*> clouds){
  Cloud* cloud_final = new Cloud();
  cloud_final->path = clouds[0]->path;
  Subset* subset;
  //---------------------------

  //ieme common subset
  for(int i=0; i<clouds[0]->nb_subset; i++){

    //We accumulate 2 frame in one
    if(i == 0 || i % 2 == 0){
      subset = new Subset();
    }

    //jeme cloud
    for(int j=0; j<clouds.size(); j++){
      Subset* subset_scala = &clouds[j]->subset[i];

      //keme points
      for(int k=0; k<subset_scala->xyz.size(); k++){
        subset->xyz.push_back(subset_scala->xyz[k]);
        subset->RGB.push_back(subset_scala->RGB[k]);
        subset->ts.push_back(subset_scala->ts[k]);
      }
    }

    if(i % 2 == 0){
      cloud_final->subset.push_back(*subset);
    }
  }

  //Remove all old clouds
  Scene sceneManager;
  for(int i=0; i<clouds.size(); i++){
    delete clouds[i];
  }

  //Load final cloud
  Loader loaderManager;
  loaderManager.load_cloud_creation(cloud_final);
  Cloud* cloud = loaderManager.get_createdcloud();

  //---------------------------
}
