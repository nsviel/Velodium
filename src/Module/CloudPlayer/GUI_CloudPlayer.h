#ifndef GUI_CLOUDPLAYER_H
#define GUI_CLOUDPLAYER_H

#include "../../common.h"

class Scene;
class Camera;
class CloudPlayer;
class Heatmap;


class GUI_CloudPlayer
{
public:
  //Constructor / Destructor
  GUI_CloudPlayer(Camera* cameraManager);
  ~GUI_CloudPlayer();

public:
  //Main function
  void design_CloudPlayer();

  //Subfunctions
  void playCloud();
  void playCloud_byMouseWheel();
  void subset_selection_bar();
  void parameter();

private:
  Scene* sceneManager;
  CloudPlayer* playerManager;
  Heatmap* heatmapManager;

};

#endif
