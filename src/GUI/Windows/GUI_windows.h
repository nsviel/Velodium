#ifndef GUI_WINDOWS_H
#define GUI_WINDOWS_H

#include "../../Load/Loader.h"
#include "../../Operation/Transformation/Transforms.h"

#include "../../common.h"

#include <iomanip>

class WIN_modifyFileInfo;
class WIN_loading;
class WIN_camera;

class Engine;
class Camera;
class Fusion;
class Filter;
class Scene;
class Attribut;
class Transforms;
class Radiometry;
class Operation;
class Extraction;
class Glyphs;
class Heatmap;
class Selection;
class Loader;
class Plotting;
class Fitting;
class Configuration;

class GUI_windows
{
public:
  //Constructor / Destructor
  GUI_windows(Engine* renderer);
  ~GUI_windows();

public:
  //Main functions
  void init();
  void window_Draw();

  //General windows
  void window_asciiData();
  void window_camera();
  void window_transformation();
  void window_selection();
  void window_extractCloud();
  void window_cutCloud();
  void window_filter();
  void window_fitting();
  void window_normal();
  void window_intensity();
  void window_color();
  void window_dataOpe();
  void window_heatmap();

  inline bool* get_cloud_movement(){return &cloud_movement;}

private:
  Loader loaderManager;
  Engine* engineManager;
  Camera* cameraManager;
  Fusion* fusionManager;
  Filter* filterManager;
  Scene* sceneManager;
  Attribut* attribManager;
  Radiometry* radioManager;
  Glyphs* glyphManager;
  Heatmap* heatmapManager;
  Selection* selectionManager;
  Plotting* plotManager;
  Fitting* fitManager;
  Operation* opeManager;
  Extraction* extractionManager;
  Transforms transformManager;
  Configuration*configManager;

  WIN_camera* wincamManager;
  WIN_loading* loadingManager;
  WIN_modifyFileInfo* fileinfoManager;

  bool cloud_movement;
};

#endif
