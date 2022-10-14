#ifndef GUI_RADIOMETRY_H
#define GUI_RADIOMETRY_H

#include "../../../common.h"

class GUI_windows;

class Engine_node;
class Scene;
class Pather;
class Radiometry;
class Heatmap;
class Reference;
class Linearization;
class GUI_module;
class Plot_radio;


class GUI_radiometry
{
public:
  //Constructor / Destructor
  GUI_radiometry(GUI_module* node_gui);
  ~GUI_radiometry();

public:
  //Main function
  void design_Radiometry();

  //Subfunctions
  void cloudStat();
  void plotting();
  void calibrationTargets();
  void options();
  void Approaches();
  void correction();

private:
  GUI_windows* gui_winManager;

  Linearization* linManager;
  Scene* sceneManager;
  Radiometry* radioManager;
  Heatmap* heatmapManager;
  Reference* refManager;
  Pather* pathManager;
  Plot_radio* radioplotManager;

  int corr_num;
  int corr_data;
  bool corr_heat;
  bool corr_ref;
};

#endif
