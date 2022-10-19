#include "Scene.h"

#include "Object.h"

#include "../Node_engine.h"
#include "../Scene/Configuration.h"

#include "../../Specific/fct_system.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Scene::Scene(Node_engine* node_engine){
  //---------------------------

  Configuration* configManager = node_engine->get_configManager();

  this->objectManager = node_engine->get_objectManager();
  this->is_visualization = configManager->parse_json_b("window", "visualization");

  this->list_cloud = new list<Cloud*>;
  this->cloud_selected = nullptr;
  this->subset_selected = nullptr;
  this->ID_cloud = 0;

  //---------------------------
}
Scene::~Scene(){
}

//Remove functions
void Scene::remove_cloud(Cloud* cloud){
  //---------------------------

  if(!get_is_list_empty()){
    int oID = cloud->oID;
    string name =  cloud->name;
    //---------------------------

    //Keep trace of the ID order
    this->selection_setCloud(oID);

    //Delete subsets
    int size = cloud->nb_subset;
    for(int i=0; i<size; i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      this->remove_subset(cloud, subset->ID);
    }

    //Delete cloud
    delete cloud;
    cloud = nullptr;

    //Delete cloud iterator in list
    list<Cloud*>::iterator it = next(list_cloud->begin(), oID);
    list_cloud->erase(it);

    //Check for end list new selected cloud
    if(oID >= list_cloud->size()){
      oID = 0;
    }

    this->update_cloud_oID(list_cloud);
    this->selection_setCloud(oID);

    //---------------------------
    string log = "Cloud "+ name +" removed";
    console.AddLog("#", log);
  }

  //If cloud list empty
  if(list_cloud->size() == 0){
    objectManager->reset_scene_object();
    this->cloud_selected = nullptr;
  }

  //---------------------------
}
void Scene::remove_cloud_all(){
  //---------------------------

  while(list_cloud->size() != 0){
    Cloud* cloud = *list_cloud->begin();
    this->remove_cloud(cloud);
  }

  //---------------------------
}
void Scene::remove_subset(Cloud* cloud, int ID){
  //---------------------------

  //Can just remove last or first subset
  Subset* subset_first = get_subset(cloud, 0);
  Subset* subset_last = get_subset(cloud, cloud->nb_subset-1);
  int oID;

  if(ID == subset_first->ID){
    oID = 0;
  }else if(ID == subset_last->ID){
    oID = cloud->nb_subset-1;
  }else{
    return;
  }

  //Supress subset objects
  Subset* subset = get_subset(cloud, oID);
  Subset* subset_buf = get_subset_buffer(cloud, oID);
  Subset* subset_ini = get_subset_init(cloud, oID);

  this->remove_subset_to_gpu(subset);

  delete subset;
  delete subset_buf;
  delete subset_ini;

  //Supress subset iterators
  list<Subset*>::iterator it = next(cloud->subset.begin(), oID);
  list<Subset*>::iterator it_buf = next(cloud->subset_buffer.begin(), oID);
  list<Subset*>::iterator it_ini = next(cloud->subset_init.begin(), oID);

  cloud->subset.erase(it);
  cloud->subset_buffer.erase(it_buf);
  cloud->subset_init.erase(it_ini);

  //---------------------------
  cloud->nb_subset = cloud->subset.size();
}
void Scene::remove_subset_to_gpu(Subset* subset){
  //---------------------------

  glDeleteBuffers(1, &subset->VBO_xyz);
  glDeleteBuffers(1, &subset->VBO_rgb);
  glDeleteBuffers(1, &subset->VBO_N);
  glDeleteVertexArrays(1, &subset->VAO);

  //---------------------------
}
void Scene::remove_subset_last(Cloud* cloud){
  //---------------------------

  //Supress subset objects
  Subset* subset = get_subset(cloud, 0);
  Subset* subset_buf = get_subset_buffer(cloud, 0);
  Subset* subset_ini = get_subset_init(cloud, 0);

  this->remove_subset_to_gpu(subset);

  //Supress subset iterators
  cloud->subset.pop_front();
  cloud->subset_buffer.pop_front();
  cloud->subset_init.pop_front();

  delete subset;
  delete subset_buf;
  delete subset_ini;

  //---------------------------
  cloud->nb_subset = cloud->subset.size();
}
void Scene::remove_subset_all(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->remove_subset(cloud, subset->ID);
  }

  //---------------------------
}

//Adding functions
void Scene::add_new_subset(Cloud* cloud, Subset* subset){
  //---------------------------
;
  //Initialize parameters
  subset->visibility = true;
  Subset* subset_buffer = new Subset(*subset);
  Subset* subset_init = new Subset(*subset);

  //Insert new subset into cloud lists
  cloud->subset.push_back(subset);
  cloud->subset_buffer.push_back(subset_buffer);
  cloud->subset_init.push_back(subset_init);

  //Update number of cloud subset
  cloud->nb_subset = cloud->subset.size();
  cloud->ID_selected = subset->ID;
  cloud->subset_selected = subset;

  objectManager->update_glyph_subset(subset);

  //---------------------------
}
void Scene::add_subset_to_gpu(Subset* subset){
  if(is_visualization){
    //---------------------------

    glGenVertexArrays(1, &subset->VAO);
    glBindVertexArray(subset->VAO);

    glGenBuffers(1, &subset->VBO_xyz);
    glGenBuffers(1, &subset->VBO_rgb);

    //Location
    glBindBuffer(GL_ARRAY_BUFFER, subset->VBO_xyz);
    glBufferData(GL_ARRAY_BUFFER, subset->xyz.size()*sizeof(glm::vec3), &subset->xyz[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
    glEnableVertexAttribArray(0);

    //Color
    glBindBuffer(GL_ARRAY_BUFFER, subset->VBO_rgb);
    glBufferData(GL_ARRAY_BUFFER, subset->RGB.size()*sizeof(glm::vec4), &subset->RGB[0], GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
    glEnableVertexAttribArray(1);

    //---------------------------
  }
}

//Reset functions
void Scene::reset_cloud(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    Subset* subset_init = get_subset_init(cloud, i);

    //Reinitialize visibility
    if(i == 0){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }

    //Reinitialize main data
    subset->xyz = subset_init->xyz;
    subset->RGB = subset_init->RGB;
    subset->N = subset_init->N;

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

  cloud->ID_selected = get_subset(cloud, 0)->ID;

  //---------------------------
  this->update_cloud_glyphs(cloud);
}
void Scene::reset_cloud_all(){
  //---------------------------

  //Reset all clouds
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    this->reset_cloud(cloud);
  }

  this->update_cloud_glyphs(cloud_selected);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Updating - cloud
void Scene::update_cloud_glyphs(Cloud* cloud){
  if(cloud == nullptr) return;
  //---------------------------

  this->update_cloud_MinMax(cloud);
  objectManager->update_glyph_cloud(cloud);

  //---------------------------
}
void Scene::update_cloud_IntensityToColor(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);

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
void Scene::update_cloud_MinMax(Cloud* cloud){
  vec3 min_cloud = vec3(100, 100, 100);
  vec3 max_cloud = vec3(-100, -100, -100);
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
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
    Subset* subset = *next(cloud->subset.begin(), i);
    this->update_subset_location(subset);
  }

  //---------------------------
}
void Scene::update_cloud_color(Cloud* cloud){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);
    this->update_subset_color(subset);
  }

  //---------------------------
}
void Scene::update_cloud_dataFormat(Cloud* cloud){
  cloud->dataFormat.clear();
  //---------------------------

  Subset* subset = get_subset(cloud, 0);
  string df = "XYZ";

  if(subset->I.size() != 0) df += " | I";
  if(subset->RGB.size() != 0) df += " | RGB";
  if(subset->N.size() != 0) df += " | N";
  if(subset->ts.size() != 0) df += " | ts";

  //---------------------------
  cloud->dataFormat = df;
}

//Updating - subset
void Scene::update_subset_glyphs(Subset* subset){
  //---------------------------

  this->update_subset_MinMax(subset);
  objectManager->update_glyph_subset(subset);

  //---------------------------
}
void Scene::update_subset(Subset* subset){
  if(subset == nullptr)return;
  //---------------------------

  this->update_subset_MinMax(subset);
  objectManager->update_glyph_subset(subset);

  //---------------------------
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

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    if(cloud->oID == ID){
      cloud_selected = cloud;
      this->update_cloud_glyphs(cloud_selected);
    }
  }

  //---------------------------
}
void Scene::selection_setCloud(Cloud* cloud){
  //---------------------------

  cloud_selected = cloud;
  this->update_cloud_glyphs(cloud_selected);

  //---------------------------
}
void Scene::selection_setSubset(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(i == ID){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }

  }

  //---------------------------
}
void Scene::selection_setNext(){
  //---------------------------

  if(list_cloud->size() != 0){
    if(cloud_selected->oID + 1 < list_cloud->size()){
      cloud_selected = *next(list_cloud->begin(),cloud_selected->oID + 1);
    }
    else{
      cloud_selected = *next(list_cloud->begin(),0);
    }
    this->update_cloud_MinMax(cloud_selected);
    objectManager->update_glyph_cloud(cloud_selected);
  }else{
    objectManager->reset_scene_object();
  }

  //---------------------------
}
void Scene::selection_cloudByName(string name){
  //---------------------------

  for (int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    if(cloud->name == name){
      cloud_selected = cloud;
      this->update_cloud_glyphs(cloud_selected);
    }
  }

  //---------------------------
}

//Assesseurs
Cloud* Scene::get_cloud_next(){
  Cloud* cloud;
  //---------------------------

  if(list_cloud->size() != 0){
    if(cloud_selected->oID + 1 < list_cloud->size()){
      cloud = *next(list_cloud->begin(),cloud_selected->oID + 1);
    }else{
      cloud = *next(list_cloud->begin(),0);
    }
  }

  //---------------------------
  return cloud;
}
Subset* Scene::get_subset_selected_init(){
  Cloud* cloud = cloud_selected;
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset_init.begin(), i);

    if(subset->ID == cloud->ID_selected){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Scene::get_subset(Cloud* cloud, int i){
  //---------------------------

  Subset* subset = *next(cloud->subset.begin(), i);

  //---------------------------
  return subset;
}
Subset* Scene::get_subset_selected(){
  //---------------------------

  if(cloud_selected != nullptr){
    int ID_subset = cloud_selected->ID_selected;

    for(int i=0; i<cloud_selected->subset.size(); i++){
      Subset* subset = *next(cloud_selected->subset.begin(), i);
      if(ID_subset == subset->ID){
        return subset;
      }
    }

  }

  //---------------------------
  return nullptr;
}
Subset* Scene::get_subset_buffer(Cloud* cloud, int i){
  //---------------------------

  Subset* subset = *next(cloud->subset_buffer.begin(), i);

  //---------------------------
  return subset;
}
Subset* Scene::get_subset_buffer_byID(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset_buffer.begin(), i);

    if(subset->ID == ID){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Scene::get_subset_init(Cloud* cloud, int i){
  //---------------------------

  Subset* subset = *next(cloud->subset_init.begin(), i);

  //---------------------------
  return subset;
}
Subset* Scene::get_subset_init_byID(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset_init.begin(), i);

    if(subset->ID == ID){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Subset* Scene::get_subset_byID(Cloud* cloud, int ID){
  //---------------------------

  if(cloud == nullptr){
    return nullptr;
  }

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->ID == ID){
      return subset;
    }
  }

  //---------------------------
  return nullptr;
}
Frame* Scene::get_frame(Cloud* cloud, int i){
  //---------------------------

  Subset* subset = *next(cloud->subset.begin(), i);
  Frame* frame = &subset->frame;

  //---------------------------
  return frame;
}
Frame* Scene::get_frame_selected(){
  //---------------------------

  if(cloud_selected != nullptr){
    int ID_subset = cloud_selected->ID_selected;

    for(int i=0; i<cloud_selected->subset.size(); i++){
      Subset* subset = *next(cloud_selected->subset.begin(), i);
      if(ID_subset == subset->ID){
        Frame* frame = &subset->frame;
        return frame;
      }
    }

  }

  //---------------------------
  return nullptr;
}
Frame* Scene::get_frame_byID(Cloud* cloud, int ID){
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = *next(cloud->subset.begin(), i);

    if(subset->ID == ID){
      Frame* frame = &subset->frame;
      return frame;
    }
  }

  //---------------------------
  return nullptr;
}
bool Scene::get_is_list_empty(){
  return list_cloud->empty();
}
int Scene::get_subset_oID(Cloud* cloud, Subset* subset){
  int oID = -1;
  //---------------------------

  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset_test = *next(cloud->subset.begin(), i);
    if(subset->ID == subset_test->ID){
      oID = i;
      break;
    }
  }

  //---------------------------
  return oID;
}
