#include "Cloud.h"

#include "../../Engine/GPU/GPU_data.h"


//Constructor / Destructor
CloudManager::CloudManager(){
  //---------------------------

  this->gpuManager = new GPU_data();

  //---------------------------
}
CloudManager::~CloudManager(){
  //---------------------------

  delete gpuManager;

  //---------------------------
}

//Main functions
