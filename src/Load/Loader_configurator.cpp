#include "Loader_configurator.h"

#include "Format/file_PTS.h"
#include "Format/file_PLY.h"
#include "Format/file_PTX.h"
#include "Format/file_PCAP.h"


//Constructor / Destructor
Loader_configurator::Loader_configurator(){
  //---------------------------

  this->ptsManager = new filePTS();
  this->ptxManager = new filePTX();

  //---------------------------
}
Loader_configurator::~Loader_configurator(){}

void Loader_configurator::load_option(int format, int option, bool value){
  //---------------------------

  switch(format){
    case 0:{//For all formats
      if(option == 0){//Intensity ON
        ptxManager->set_retrievingIntensity(value);
        ptsManager->set_retrievingIntensity(value);
      }else
      if(option == 1){//Color ON
        ptxManager->set_retrievingColor(value);
        ptsManager->set_retrievingColor(value);
      }else
      if(option == 2){//Normal ON
        ptxManager->set_retrievingNormal(value);
        ptsManager->set_retrievingNormal(value);
      }else
      if(option == 3){//I format 01
        ptxManager->set_IdataFormat(0);
        ptsManager->set_IdataFormat(0);
      }else
      if(option == 4){//I format 255
        ptxManager->set_IdataFormat(1);
        ptsManager->set_IdataFormat(1);
      }else
      if(option == 5){//I format 4096
        ptxManager->set_IdataFormat(2);
        ptsManager->set_IdataFormat(2);
      }
      break;
    }

    case 1:{//PTS options
      break;
    }

    case 2:{//PTX options
      if(option == 0){
        ptxManager->set_scannerAtOrigin(value);
      }else
      if(option == 1){
        ptxManager->set_applyCloudTransfo(value);
      }else
      if(option == 2){
        ptxManager->set_separateCloud(value);
      }
      if(option == 3){
        ptxManager->set_notUseZValue(value);
      }
      break;
    }
  }

  //---------------------------
}
