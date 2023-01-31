//---------------------------------------------
// Possible ARG command:
// - ai
// - capture
// - car
// - train
//---------------------------------------------
#include "Engine/OpenGL/CoreGLengine.h"


int main(int argc, char* argv[])
{
  std::cout<<"--- \033[1;34mBegin Lidium program\033[0m ---"<<std::endl;
  //---------------------------
  
  CoreGLengine GLengine;
  GLengine.init();
  GLengine.arg(argc, argv);
  GLengine.loop();

  //---------------------------
  std::cout<<"--- \033[1;34mClose Lidium program\033[0m ---"<<std::endl;
  return 0;
}
