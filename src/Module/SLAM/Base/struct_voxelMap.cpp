#include "struct_voxelMap.h"


int slamap::get_signature(int kx, int ky, int kz){
  //---------------------------

  int key = (kx*2000 + ky)*1000 + kz;
  if(key >= INT_MAX || key <= INT_MIN){
    std::cout<<"[error] Int min or max value for key"<<std::endl;
  }

  //---------------------------
  return key;
}
void slamap::reset(){
  //---------------------------

  this->map.clear();
  this->cloud.clear();

  this->linked_col_ID = -1;
  this->linked_subset_ID = -1;
  this->current_frame_ID = 0;

  //---------------------------
}
