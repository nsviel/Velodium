#ifndef GUI_VELODYNE_H
#define GUI_VELODYNE_H

class Velodyne;

#include "../../common.h"


class GUI_Velodyne
{
public:
  //Constructor / Destructor
  GUI_Velodyne();
  ~GUI_Velodyne();

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
