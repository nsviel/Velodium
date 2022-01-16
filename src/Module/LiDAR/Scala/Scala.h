#ifndef SCALA_H
#define SCALA_H

#include "../../../common.h"


class Scala
{
public:
  //Constructor / Destructor
  Scala();
  ~Scala();

public:
  void loading(string pathDir);

  vector<string> loading_allPathDir(string pathDir);
  vector<Cloud*> loading_allFile(vector<string> allpath);
  Cloud* loading_reoganizeData(vector<Cloud*> clouds);
  void compute_relativeTimestamp(Cloud* cloud);

private:
};

#endif
