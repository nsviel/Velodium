#ifndef GUI_REGISTRATION_H
#define GUI_REGISTRATION_H

#include "../../../common.h"

class Scene;
class Node_engine;
class ICP;
class Registration;
class Glyphs;
class Pose;
class Module_registration;
class Pose;


class GUI_registration
{
public:
  //Constructor / Destructor
  GUI_registration(Module_registration* module);
  ~GUI_registration();

public:
  //Main function
  void design_Registration();

  //Subfunctions
  void regist_Color();
  void regist_DOF();
  void regist_Parameters();
  void regist_Registration();
  void regist_Stats();

private:
  Pose* poseManager;
  Scene* sceneManager;
  ICP* icpManager;
  Registration* regisManager;
  Glyphs* glyphManager;

  int regis_algo;
};

#endif
