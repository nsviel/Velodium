#include "Argument.h"
#include "Node_engine.h"
#include "Scene/Configuration.h"

#include "../Load/Node_load.h"
#include "../Load/Processing/Loader.h"


//Constructor / Destructor
Argument::Argument(Node_engine* node_engine){
  //---------------------------

  Node_load* node_load = node_engine->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->loadManager = node_load->get_loaderManager();

  //---------------------------
}
Argument::~Argument(){}

void Argument::process_arg(int argc, char* argv[]){
  //---------------------------

  //Retrieve all arguments
  vector<string> arg_vec;
  for(int i=1; i<argc; i++){
    arg_vec.push_back(argv[i]);
  }

  //Command line processing
  if(argc > 1){
    if(arg_vec[0] == "capture"){
      configManager->make_preconfig(1);
    }
    else if(arg_vec[0] == "ai"){
      configManager->make_preconfig(2);
    }
    else if(arg_vec[0] == "server"){
      configManager->make_preconfig(3);
    }
    else if(arg_vec[0] == "load" && argc == 3){
      loadManager->load_cloud(arg_vec[1]);
    }
  }

  //---------------------------
}
