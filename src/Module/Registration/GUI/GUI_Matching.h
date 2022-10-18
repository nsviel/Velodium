#ifndef GUI_MATCHING_H
#define GUI_MATCHING_H

#include "../../../common.h"

class Module_registration;
class Scene;
class Glyphs;
class Attribut;
class ICP;
class Keypoint;
class Radiometry;
class Linearization;


class GUI_matching
{
public:
  GUI_matching(Module_registration* module);
  ~GUI_matching();

public:
  //Main function
  void design_Matching();

  //Subcategories
  void match_matching();
  void match_keypoint();
  void match_dense();
  void match_methods();

  //Subfunctions
  void keypoint_options();
  void keypoint_parameters();
  void keypoint_rejection();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Attribut* attribManager;
  ICP* icpManager;
  Keypoint* keyManager;
  Radiometry* radioManager;
};

#endif
