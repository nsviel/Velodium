#ifndef EXTRACTION_H
#define EXTRACTION_H

#include "../../Module/Registration/struct_misc.h"
#include "../../common.h"

class Node_operation;
class Scene;
class Loader;
class Attribut;
class Object;

struct subpart{
  //---------------------------

  int ID;
  std::string name;
  std::string namePC;
  glm::vec3 COM;
  glm::vec3 minloc;
  glm::vec3 maxloc;

  //---------------------------
};


class Extraction
{
public:
  //Constructor / Destructor
  Extraction(Node_operation* node_ope);
  ~Extraction();

public:
  //Extract / Cutting function
  void fct_extractCloud(Collection* collection);
  void fct_extractSelected(Collection* collection);
  void fct_cutCloud(Cloud* cloud);
  void fct_cutCloud_all();
  void fct_selectPart(Cloud* cloud, vec3 min, vec3 max);
  void supress_selectedpart(subpart* part);

  //Merging function
  void fct_merging_list(vector<Collection*> list_part);
  void fct_merging_newCloud(Collection* cloud_1, Collection* cloud_2);
  void fct_merging_addCloud(Collection* cloud_1, Collection* cloud_2);

  //Selection function
  void fct_highlighting(Cloud* cloud, Cloud* list_obj_init);
  void set_AABB_min(vec3 min_in);
  void set_AABB_max(vec3 max_in);

  //Setters / Getters
  inline void set_sliceON(bool value){this->sliceON = value;}
  inline bool* get_highlightON(){return &highlightON;}
  inline list<subpart*>* get_listParts(){return list_part;}

private:
  Scene* sceneManager;
  Loader* loaderManager;
  Object* objectManager;
  Attribut* attribManager;

  list<subpart*>* list_part;
  bool highlightON;
  bool sliceON;
  int ID_cloud;
  int ID_part;
};

#endif
