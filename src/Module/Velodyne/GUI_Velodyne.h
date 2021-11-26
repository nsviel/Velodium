#ifndef GUI_VELODYNE_H
#define GUI_VELODYNE_H

class Velodyne;

#include "../../common.h"



class GUI_velodyne
{
public:
  //Constructor / Destructor
  GUI_velodyne();
  ~GUI_velodyne();

public:
  //Main function
  void design_Velodyne();

  //Subfunctions
  void Start();
  void State();
  void Parameters();
  void Capture();
  void Recording();

private:
  Velodyne* veloManager;


};

#endif
