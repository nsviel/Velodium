#ifndef SCALA_H
#define SCALA_H

#include "../../../../common.h"

class Engine_node;
class Scene;
class Loader;


class Scala
{
public:
  //Constructor / Destructor
  Scala(Engine_node* node_engine);
  ~Scala();

public:
  void loading(string pathDir);

  vector<string> loading_allPathDir(string pathDir);
  vector<Cloud*> loading_allFile(vector<string> allpath);
  Cloud* loading_reoganizeData(vector<Cloud*> clouds);
  void compute_relativeTimestamp(Cloud* cloud);

private:
  Scene* sceneManager;
  Loader* loaderManager;
};

#endif
