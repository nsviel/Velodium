#ifndef SCALA_FILE_H
#define SCALA_FILE_H

#include "../../../../common.h"

class Node_engine;
class Scene;
class Loader;


class Scala_file
{
public:
  //Constructor / Destructor
  Scala_file(Node_engine* node_engine);
  ~Scala_file();

public:
  void loading(string pathDir);

  vector<string> loading_allPathDir(string pathDir);
  vector<Collection*> loading_allFile(vector<string> allpath);
  Collection* loading_reoganizeData(vector<Collection*> clouds);
  void compute_relativeTimestamp(Collection* collection);

private:
  Scene* sceneManager;
  Loader* loaderManager;
};

#endif
