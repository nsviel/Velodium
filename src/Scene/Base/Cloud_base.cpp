#include "Cloud_base.h"


//Constructor / Destructor
Cloud_base::Cloud_base(){
  //---------------------------

  //---------------------------
}
Cloud_base::~Cloud_base(){}

//Main function
Subset* Cloud_base::get_subset_byID(int ID){
  //---------------------------

  for(int i=0; i<subset.size(); i++){
    Subset* sub = *next(subset.begin(), i);

    if(sub->ID == ID){
      return sub;
    }
  }

  //---------------------------
  return subset_selected;
}
