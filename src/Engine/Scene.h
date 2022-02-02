#ifndef SCENE_H
#define SCENE_H

#include "../common.h"

#include "Data/Database.h"
extern struct Database database;

class Data;


class Scene
{
public:
  //Constructor / Destructor
  Scene();
  ~Scene();

public:
  //Remove functions
  void remove_cloud(Cloud* cloud);
  void remove_cloud_all();
  void remove_subset(Cloud* cloud, int ID);

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

  //Assesseurs
  Cloud* get_cloud_selected();
  Cloud* get_cloud_next();
  Cloud* get_cloud_byName(string name);
  Cloud* get_cloud_byoID(int oID);

  Subset* get_subset_selected();
  Subset* get_subset_selected_init();
  Subset* get_subset(Cloud* cloud, int i);
  Subset* get_subset_buffer(Cloud* cloud, int i);
  Subset* get_subset_init(Cloud* cloud, int i);
  Subset* get_subset_byID(Cloud* cloud, int ID);
  Subset* get_subset_buffer_byID(Cloud* cloud, int ID);

  Frame* get_frame(Cloud* cloud, int i);
  Frame* get_frame_byID(Cloud* cloud, int ID);

  int get_orderSelectedcloud();
  int get_listcloudSize();
  vector<string> get_nameByOrder();
  bool is_cloudExist(Cloud* cloud);
  bool is_cloudNameExist(Cloud* cloud);
  bool is_atLeastMinNbcloud(int nbMin);
  bool is_listcloudEmpty();
  bool is_atLeastOnecloud();

  inline list<Glyph*>* get_list_glyph(){return database.list_glyph;}
  inline list<Cloud*>* get_list_cloud(){return database.list_cloud;}
  inline int* get_list_ID_cloud(){return &database.ID_cloud;}
  inline int* get_list_ID_glyph(){return &database.ID_glyph;}
  inline void set_selected_cloud(Cloud* cloud){database.cloud_selected = cloud;}

private:
  Data* dataManager;
};

#endif
