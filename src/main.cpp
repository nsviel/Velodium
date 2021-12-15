#include "Engine/OpenGL/CoreGLengine.h"

/**
 * \file main.cpp
 * \brief Start the software
 */

int main(int argc, char *argv[])
{
  std::cout<<"--Begin program--"<<std::endl;
  //---------------------------

  CoreGLengine GLengine;
  GLengine.init();
  GLengine.loop();

  //---------------------------
  std::cout<<"--Close program--"<<std::endl;
  return 0;
}
