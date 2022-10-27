#ifndef MOD_EXTRACTION_H
#define MOD_EXTRACTION_H

#include "../../../common.h"

class Scene;
class Glyphs;
class Node_operation;
class Extraction;


class MOD_extraction
{
public:
  //Constructor / Destructor
  MOD_extraction(Node_operation* node_ope);
  ~MOD_extraction();

public:
  void design_extractCloud();

  //Specific function
  void extract_with_aabb();
  void extract_with_mouse();
  void merge_cloud();

private:
  Scene* sceneManager;
  Glyphs* glyphManager;
  Extraction* extractionManager;

  int item_width;
};

#endif
