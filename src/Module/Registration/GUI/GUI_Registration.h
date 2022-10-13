#ifndef GUI_REGISTRATION_H
#define GUI_REGISTRATION_H

#include "../../../common.h"

class Scene;
class Engine_node;
class ICP;
class Registration;
class Glyphs;
class Transforms;
class GUI_module;


class GUI_registration
{
public:
  //Constructor / Destructor
  GUI_registration(GUI_module* node_gui);
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
  Scene* sceneManager;
  ICP* icpManager;
  Registration* regisManager;
  Glyphs* glyphManager;
  Transforms* transformManager;

  int regis_algo;
};

#endif
