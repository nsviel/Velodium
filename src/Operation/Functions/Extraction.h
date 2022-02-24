#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "../../Engine/Data/struct_generic.h"
#include "../../common.h"

class Operation_node;
class Scene;
class Loader;
class Attribut;


class Extraction
{
public:
  //Constructor / Destructor
  Extraction(Operation_node* node_ope);
  ~Extraction();

public:
  //Extraction
  void fct_extractCloud(Cloud* cloud);
  void fct_extractSelected(Cloud* cloud);
  void fct_cutCloud(Subset* subset);
  void fct_cutCloud_all();
  void fct_highlighting(Subset* subset, Subset* subset_init);
  void fct_merging_list(vector<Cloud*> list_part);
  void fct_merging_newCloud(Cloud* cloud_1, Cloud* cloud_2);
  void fct_merging_addCloud(Cloud* cloud_1, Cloud* cloud_2);

  //Selection subparts
  void fct_selectPart(Subset* subset, vec3 min, vec3 max);
  void supress_selectedpart(subpart* part);

  //Subfunctions
  void set_AABB_min(vec3 min_in);
  void set_AABB_max(vec3 max_in);

  //Setters / Getters
  inline void set_sliceON(bool value){this->sliceON = value;}
  inline bool* get_highlightON(){return &highlightON;}
  inline list<subpart*>* get_listParts(){return list_part;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Attribut* attribManager;

  list<subpart*>* list_part;
  bool highlightON;
  bool sliceON;
  int ID_cloud;
  int ID_part;
};

#endif
