#ifndef SLAM_GLYPH_H
#define SLAM_GLYPH_H

#include "../../../common.h"

class SLAM;
class SLAM_map;
class Object;


class SLAM_glyph
{
public:
  //Constructor / Destructor
  SLAM_glyph(SLAM* slam);
  ~SLAM_glyph();

public:
  //Main function
  void update_glyph(Subset* subset);
  void reset_glyph();

  //Subfunctions$
  void update_glyph_keypoint(Subset* subset);
  void update_glyph_nn(Subset* subset);
  void update_glyph_normal(Subset* subset);
  void update_glyph_map();
  
private:
  Object* objectManager;
  SLAM_map* slam_map;
};


#endif
