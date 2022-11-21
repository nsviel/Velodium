#ifndef GUI_RADIOMETRY_H
#define GUI_RADIOMETRY_H

#include "../../../common.h"

class Module_radiometry;
class Scene;
class Radiometry;
class Heatmap;
class Reference;
class Linearization;
class Plot_radio;


class GUI_radiometry
{
public:
  //Constructor / Destructor
  GUI_radiometry(Module_radiometry* module);
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
  Linearization* linManager;
  Scene* sceneManager;
  Radiometry* radioManager;
  Heatmap* heatmapManager;
  Reference* refManager;
  Plot_radio* radioplotManager;

  int corr_num;
  int corr_data;
  bool corr_heat;
  bool corr_ref;
};

#endif
