#include "Argument.h"

#include "../Node_engine.h"
#include "../Scene/Configuration.h"
#include "../Camera/Camera.h"

#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"


//Constructor / Destructor
Argument::Argument(Node_engine* node){
  this->node_engine = node;
  //---------------------------

  Node_load* node_load = node_engine->get_node_load();

  this->configManager = node_engine->get_configManager();
  this->loadManager = node_load->get_loaderManager();
  this->cameraManager = node_engine->get_cameraManager();

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
    //Capture mode
    if(arg_vec[0] == "capture"){
      configManager->make_preconfig(1);
      node_engine->update();
    }
    //AI interface mode
    else if(arg_vec[0] == "ai"){
      configManager->make_preconfig(2);
      node_engine->update();
    }
    //Server mode
    else if(arg_vec[0] == "server"){
      configManager->make_preconfig(3);
      node_engine->update();
    }
    //Load at start
    else if(arg_vec[0] == "load" && argc == 3){
      cameraManager->set_mode_view(1);
      loadManager->load_cloud(arg_vec[1]);
    }
    //Help
    else{
      cout << endl;
      cout << "[Help] Possible commands are:" << endl;
      cout << "   \033[1;32mcapture\033[0m                 [LiDAR capture mode]" << endl;
      cout << "   \033[1;32mai\033[0m                      [AI interface mode]" << endl;
      cout << "   \033[1;32mserver\033[0m                  [Server mode]" << endl;
      cout << "   \033[1;32mload + absolute_path\033[0m    [Load a cloud at start]" << endl;
      cout << endl;
      exit(0);
    }
  }

  //---------------------------
}
