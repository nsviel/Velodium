#ifndef OPERATION_H
#define OPERATION_H

class Scene;
class Loader;
class Glyphs;

#include "../common.h"

class Operation
{
public:
  //Constructor / Destructor
  Operation();
  ~Operation();

public:
  void reset();
  void fastScene(int mode);
  void loading();
  void loading_frames();
  void loading_directoryFrames(string path);
  void selectDirectory(string& path);
  void loading_sampling();
  void loading_treatment();
  vector<string> get_directoryAllFilePath(string path);
  void samplingLoader(string path);
  void saving();
  void allSaving();
  void convertIscale();

  inline void set_spaceSampling(float value){this->spaceSampling = value;}
  inline void set_nbLineSampling(int value){this->nbLineSampling = value;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Glyphs* glyphManager;

  float spaceSampling;
  int nbLineSampling;
  string pathDir;
  uint modelID, comID;
};

#endif
