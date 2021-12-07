#ifndef SCENE_H
#define SCENE_H

/**
 * \file Scene.h
 * \brief Point cloud management
 */

#include "../common.h"

class Data;


class Scene
{
public:
  //Constructor / Destructor
  Scene();
  ~Scene();

public:
  //Remove functions
  void removeCloud(Cloud* cloud);
  void removeCloud_all();

  //Update data
  void update_cloud_glyphs(Cloud* cloud);
  void update_cloud_IntensityToColor(Cloud* cloud);
  void update_cloud_oID(list<Cloud*>* list);
  void update_cloud_reset(Cloud* cloud);
  void update_cloud_MinMax(Cloud* cloud);
  void update_cloud_location(Cloud* cloud);
  void update_cloud_color(Cloud* cloud);

  void update_subset_glyphs(Subset* subset);
  void update_subset(Subset* subset);
  void update_subset_IntensityToColor(Subset* subset);
  void update_subset_MinMax(Subset* subset);
  void update_subset_location(Subset* subset);
  void update_subset_color(Subset* subset);
  void update_subset_dataFormat(Subset* subset);

  //Subfunctions
  void selection_setNext();
  void selection_setCloud(int ID);
  void selection_setSubset(Cloud* cloud, int ID);
  void set_cloudVisibility(Cloud* cloud, bool visible);
  void selection_cloudByName(string name);
  void selection_setCloud(Cloud* cloud);

  vector<string> get_nameByOrder();
  Subset* get_visibleSubset();
  Subset* get_subset_selected();
  Cloud* get_othercloud();
  Cloud* get_cloudByName(string name);
  Cloud* get_cloudByOID(int ID);
  int get_orderSelectedcloud();
  int get_listcloudSize();

  bool is_cloudExist(Cloud* cloud);
  bool is_cloudNameExist(Cloud* cloud);
  bool is_atLeastMinNbcloud(int nbMin);
  bool is_listcloudEmpty();
  bool is_atLeastOnecloud();

private:
  Data* dataManager;
};

#endif
