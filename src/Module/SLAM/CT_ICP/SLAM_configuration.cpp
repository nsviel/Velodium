#include "SLAM_configuration.h"

#include "SLAM_normal.h"
#include "SLAM_optim_gn.h"

#include "../Slam.h"


//Constructor / Destructor
SLAM_configuration::SLAM_configuration(Slam* slam){
  //---------------------------

  this->slamManager = slam;
  this->normalManager = slam->get_SLAM_normal();
  this->gnManager = slam->get_SLAM_optim_gn();

  //---------------------------
}
SLAM_configuration::~SLAM_configuration(){}

void SLAM_configuration::set_nb_thread(int value){
  //---------------------------

  slamManager->set_nb_thread(value);
  normalManager->set_nb_thread(value);
  gnManager->set_nb_thread(value);

  //---------------------------
}
