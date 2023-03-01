#ifndef SUBSET_BASE_H
#define SUBSET_BASE_H

#include "Cloud_base.h"


class Subset_base : public Cloud_base
{
public:
  //Constructor / Destructor
  Subset_base();
  ~Subset_base();

public:


protected:
  int ID;

  //Own glyphs and structures
  std::map<std::string, Glyph> glyphs;
  Frame frame;
  Data_icp icp;
  Data_pred detection;
  Data_pred detection_gt;

};

#endif
