#include "Argument.h"
#include "Node_engine.h"
#include "Scene/Configuration.h"


//Constructor / Destructor
Argument::Argument(Node_engine* engine){
  //---------------------------

  this->configManager = new Configuration();

  //---------------------------
}
Argument::~Argument(){}

void Argument::process_arg(int argc, char* argv[]){
  //---------------------------

  //Command line processing
  if(argc > 1){
    string arg_1 = argv[1];

    if(arg_1 == "capture"){
      configManager->make_preconfig(1);
    }
    else if(arg_1 == "ai"){
      configManager->make_preconfig(2);
    }
    else if(arg_1 == "server"){
      configManager->make_preconfig(3);
    }
  }

  //---------------------------
}
