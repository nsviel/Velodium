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
  void update_glyph(Cloud* cloud, Subset* subset);
  void update_visibility(Subset* subset);
  void reset_glyph();

  //Subfunctions$
  void update_glyph_keypoint(Subset* subset);
  void update_glyph_nn(Subset* subset);
  void update_glyph_matching(Subset* subset);
  void update_glyph_normal(Subset* subset);
  void update_glyph_map();
  void update_glyph_car(Cloud* cloud);
  void update_glyph_trajectory(Cloud* cloud);

  inline vec4* get_localcloud_color(){return &localcloud->color_unique;}
  inline bool* get_with_keypoint(){return &with_keypoint;}
  inline bool* get_with_neighbor(){return &with_neighbor;}
  inline bool* get_with_matching(){return &with_matching;}
  inline bool* get_with_trajectory(){return &with_trajectory;}
  inline bool* get_with_localmap(){return &with_localmap;}
  inline bool* get_with_localcloud(){return &with_localcloud;}
  inline bool* get_with_car(){return &with_car;}

private:
  SLAM_map* slam_map;
  Object* objectManager;

  Glyph* trajectory;
  Glyph* localmap;
  Glyph* localcloud;
  Glyph* car;
  Glyph* matching;
  Glyph* keypoint;

  bool with_keypoint;
  bool with_neighbor;
  bool with_matching;
  bool with_trajectory;
  bool with_localmap;
  bool with_localcloud;
  bool with_car;
};


#endif
