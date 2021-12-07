#include "Scene.h"

#include "Glyphs.h"

#include "../Specific/fct_opengl.h"
#include "../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Scene::Scene(){
  //---------------------------

  //---------------------------
}
Scene::~Scene(){
}

//Remove functions
void Scene::removeCloud(Cloud* cloud){
  //---------------------------

  if(is_atLeastOnecloud()){
    int oID = cloud->oID;
    string name =  cloud->name;
    //---------------------------

    //Keep trace of the ID order
    this->selection_setCloud(oID);

    //Delete cloud
    /*for(int i=0; i<cloud->nb_subset; i++){
      delete cloud->subset
    }*/
    delete database.cloud_selected;
    //delete cloud;

    //Delete cloud iterator in list
    list<Cloud*>::iterator it = next(database.list_cloud->begin(), oID);
    database.list_cloud->erase(it);

    //Check for end list new selected cloud
    if(oID >= database.list_cloud->size()){
      oID = 0;
    }

    this->update_cloud_oID(database.list_cloud);
    this->selection_setCloud(oID);

    //---------------------------
    console.AddLog("Cloud %s removed", name.c_str());
  }
  if(database.list_cloud->size() == 0){
    Glyphs glyphManager;
    glyphManager.reset();
  }

  //---------------------------
}
void Scene::removeCloud_all(){
  //---------------------------

  while(database.list_cloud->size() != 0){
    Cloud* cloud = *database.list_cloud->begin();
    this->removeCloud(cloud);
  }

  //---------------------------
}

//Updating
void Scene::update_cloud_glyphs(Cloud* cloud){
  if(cloud == nullptr)return;
  //---------------------------

  this->update_cloud_MinMax(cloud);
  Glyphs glyphManager;
  glyphManager.update(cloud);

  //---------------------------
}
void Scene::update_cloud_IntensityToColor(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];

    vector<float>& Is = subset->I;
    vector<vec4>& RGB = subset->RGB;

    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i], 1.0f);
    }

    this->update_subset_color(subset);
  }

  //---------------------------
}
void Scene::update_cloud_oID(list<Cloud*>* list){
  //---------------------------

  for(int i=0; i<list->size(); i++){
    Cloud* cloud = *next(list->begin(),i);
    if(cloud->oID != i) cloud->oID = i;
  }

  //---------------------------
}
void Scene::update_cloud_reset(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];

    //Reinitialize main data
    subset->xyz = cloud->subset_init[i].xyz;
    subset->RGB = cloud->subset_init[i].RGB;
    subset->N = cloud->subset_init[i].N;

    //Reset additional data
    subset->R.clear();
    subset->It.clear();
    subset->cosIt.clear();
    subset->root = vec3(0,0,0);

    //Transformation matrices
    subset->scale = mat4(1.0);
    subset->trans = mat4(1.0);
    subset->rotat = mat4(1.0);

    //Update
    this->update_subset_MinMax(subset);
    this->update_subset_location(subset);
    this->update_subset_color(subset);

    //Reset frame
    subset->frame.reset();
  }

  //---------------------------
  this->update_cloud_glyphs(cloud);
}
void Scene::update_cloud_MinMax(Cloud* cloud){
  vec3 min_cloud = vec3(100, 100, 100);
  vec3 max_cloud = vec3(-100, -100, -100);
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];
    this->update_subset_MinMax(subset);

    //Cloud
    for(int j=0; j<3; j++){
      if ( min_cloud[j] > subset->min[j] ) min_cloud[j] = subset->min[j];
      if ( max_cloud[j] < subset->max[j] ) max_cloud[j] = subset->max[j];
    }
  }

  //---------------------------
  cloud->min = min_cloud;
  cloud->max = max_cloud;
}
void Scene::update_cloud_location(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];
    this->update_subset_location(subset);
  }

  //---------------------------
}
void Scene::update_cloud_color(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = &cloud->subset[i];
    this->update_subset_color(subset);
  }

  //---------------------------
}

void Scene::update_subset_glyphs(Subset* subset){
  //---------------------------

  this->update_subset_MinMax(subset);
  Glyphs glyphManager;
  glyphManager.update(subset);

  //---------------------------
}
void Scene::update_subset(Subset* subset){
  if(subset == nullptr)return;
  //---------------------------

  this->update_subset_MinMax(subset);
  Glyphs glyphManager;
  glyphManager.update(subset);

  //---------------------------
}
void Scene::update_subset_dataFormat(Subset* subset){
  subset->dataFormat.clear();
  //---------------------------

  subset->nb_point = subset->xyz.size();

  string df = "XYZ";
  if(subset->I.size() != 0) df += " | I";
  if(subset->has_color) df += " | RGB";
  if(subset->N.size() != 0) df += " | N";
  if(subset->ts.size() != 0) df += " | ts";

  //---------------------------
  subset->dataFormat = df;
}
void Scene::update_subset_IntensityToColor(Subset* subset){
  //---------------------------

  vector<float>& Is = subset->I;
  vector<vec4>& RGB = subset->RGB;
  RGB.clear();

  for(int i=0; i<Is.size(); i++){
    vec4 new_color = vec4(Is[i], Is[i], Is[i], 1.0f);
    RGB.push_back(new_color);
  }

  this->update_subset_color(subset);

  //---------------------------
}
void Scene::update_subset_MinMax(Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vec3 centroid = vec3(0, 0, 0);
  vec3 min = XYZ[0];
  vec3 max = XYZ[0];
  //---------------------------

  for(int i=0; i<XYZ.size(); i++){
    for(int j=0; j<3; j++){
      if ( XYZ[i][j] <= min[j] ) min[j] = XYZ[i][j];
      if ( XYZ[i][j] >= max[j] ) max[j] = XYZ[i][j];
      centroid[j] += XYZ[i][j];
    }
  }

  for(int j=0;j<3;j++){
    centroid[j] /= XYZ.size();
  }

  //---------------------------
  subset->min = min;
  subset->max = max;
  subset->COM = centroid;
}
void Scene::update_subset_location(Subset* subset){
  //---------------------------

  //Reactualise vertex position data
  vector<vec3>& XYZ = subset->xyz;
  glBindBuffer(GL_ARRAY_BUFFER, subset->VBO_xyz);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
void Scene::update_subset_color(Subset* subset){
  //---------------------------

  //Reactualise vertex color data
  vector<vec4>& RGB = subset->RGB;
  glBindBuffer(GL_ARRAY_BUFFER, subset->VBO_rgb);
  glBufferData(GL_ARRAY_BUFFER, RGB.size() * sizeof(glm::vec4), &RGB[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}

//Selection
void Scene::selection_setCloud(int ID){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    if(cloud->oID == ID){
      database.cloud_selected = cloud;
      this->update_cloud_glyphs(database.cloud_selected);
    }
  }

  //---------------------------
}
void Scene::selection_setCloud(Cloud* cloud){
  //---------------------------

  database.cloud_selected = cloud;
  this->update_cloud_glyphs(database.cloud_selected);

  //---------------------------
}
void Scene::selection_setSubset(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];

    if(i == ID){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }

  }

  //---------------------------
}
void Scene::selection_setNext(){
  Glyphs glyphManager;
  //---------------------------

  if(database.list_cloud->size() != 0){
    if(database.cloud_selected->oID + 1 < database.list_cloud->size()){
      database.cloud_selected = *next(database.list_cloud->begin(),database.cloud_selected->oID + 1);
    }
    else{
      database.cloud_selected = *next(database.list_cloud->begin(),0);
    }
    this->update_cloud_MinMax(database.cloud_selected);
    glyphManager.update(database.cloud_selected);
  }else{
    glyphManager.reset();
  }

  //---------------------------
}
void Scene::selection_cloudByName(string name){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);

    if(cloud->name == name){
      database.cloud_selected = cloud;
      this->update_cloud_glyphs(database.cloud_selected);
    }
  }

  //---------------------------
}

//Specific functions
vector<string> Scene::get_nameByOrder(){
  vector<string> nameByOrder;
  //---------------------------

  if(is_atLeastOnecloud()){
    string temp;

    for(int i=0; i<database.list_cloud->size(); i++){
      Cloud* cloud = *next(database.list_cloud->begin(), i);
      nameByOrder.push_back(cloud->name);
    }

    bool ok = true;
    while(ok){
      ok = false;
      for(int i=0; i<database.list_cloud->size()-1; i++){
        int id = strcasecmp_withNumbers(nameByOrder[i].c_str(), nameByOrder[i+1].c_str());
        if(id > 0){
          temp = nameByOrder[i];
          nameByOrder[i] = nameByOrder[i+1];
          nameByOrder[i+1] = temp;

          ok = true;
        }
      }
    }

    //Reorganize list of cloudes
    list<Cloud*>* list_out = new list<Cloud*>;
    for(int i=0; i<nameByOrder.size(); i++){
      list_out->push_back(get_cloudByName(nameByOrder[i]));
    }

    database.list_cloud = list_out;
  }

  //---------------------------
  return nameByOrder;
}
Subset* Scene::get_visibleSubset(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = &cloud->subset[i];

    if(subset->visibility){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Scene::get_subset_selected(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  Subset* subset = &cloud->subset[cloud->subset_selected];

  //---------------------------
  return subset;
}
Cloud* Scene::get_othercloud(){
  Cloud* cloud;
  //---------------------------

  if(database.list_cloud->size() != 0){
    if(database.cloud_selected->oID + 1 < database.list_cloud->size()){
      cloud = *next(database.list_cloud->begin(),database.cloud_selected->oID + 1);
    }else{
      cloud = *next(database.list_cloud->begin(),0);
    }
  }

  //---------------------------
  return cloud;
}
Cloud* Scene::get_cloudByName(string name){
  Cloud* cloud_out;
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);

    if(cloud->name == name){
      cloud_out = cloud;
    }
  }

  //---------------------------
  return cloud_out;
}
Cloud* Scene::get_cloudByOID(int oID){
  Cloud* cloud_out;
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);

    if(cloud->oID == oID){
      cloud_out = cloud;
    }
  }

  //---------------------------
  return cloud_out;
}
int Scene::get_orderSelectedcloud(){
  vector<string> Names = get_nameByOrder();
  //---------------------------

  int order = -1;
  for(int i=0; i<Names.size(); i++){
    if(database.cloud_selected->name == Names[i]){
      order = i;
    }
  }

  //---------------------------
  return order;
}
int Scene::get_listcloudSize(){
  return database.list_cloud->size();
}
void Scene::set_cloudVisibility(Cloud* cloud, bool visibleON){
  //---------------------------

  //Toggle cloud visibility
  if(visibleON == true && cloud->visibility == false){
    cloud->visibility = true;
  }else if(visibleON == false && cloud->visibility == true){
    cloud->visibility = false;
  }

  //---------------------------
}

bool Scene::is_cloudExist(Cloud* cloud_in){
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);

    if(cloud_in->ID == cloud->ID){
      return true;
    }
  }

  //---------------------------
  cout<<"Cloud ID problem..."<<endl;
  return false;
}
bool Scene::is_cloudNameExist(Cloud* cloud_in){
  bool exist = false;
  //---------------------------

  for (int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);

    if(cloud->name == cloud_in->name){
      cloud_in->name = cloud_in->name + "_";
      exist = true;
    }
  }

  //---------------------------
  return exist;
}
bool Scene::is_atLeastMinNbcloud(int nbMin){
  bool result = false;
  int nb = database.list_cloud->size();
  //---------------------------

  if(nb >= nbMin){
    result = true;
  }

  //---------------------------
  return result;
}
bool Scene::is_listcloudEmpty(){
  return database.list_cloud->empty();
}
bool Scene::is_atLeastOnecloud(){
  return !database.list_cloud->empty();
}
