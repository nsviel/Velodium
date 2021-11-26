#ifndef DATABASE_H
#define DATABASE_H

#include "struct_glyph.h"
#include "struct_cloud.h"

#include <list>


struct Database{
  std::list<Cloud*>* list_cloud;
  std::list<Glyph*>* list_glyph;

  Cloud* cloud_selected;
  int ID_cloud;
  int ID_glyph;
};



#endif
