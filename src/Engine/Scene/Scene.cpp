#include "Scene.h"

#include "Glyphs.h"

#include "../../Specific/fct_system.h"
#include "../../Operation/Transformation/Transforms.h"


//Constructor / Destructor
Scene::Scene(){
  //---------------------------

  //---------------------------
}
Scene::~Scene(){
}

//Remove functions
void Scene::exit(){
  //---------------------------

  GLFWwindow* window = glfwGetCurrentContext();
  glfwSetWindowShouldClose(window, true);

  //---------------------------
}
void Scene::remove_cloud(Cloud* cloud){
  //---------------------------

  if(is_atLeastOnecloud()){
    int oID = cloud->oID;
    string name =  cloud->name;
    //---------------------------

    //Keep trace of the ID order
    this->selection_setCloud(oID);

    //Delete subsets
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      this->remove_subset(cloud, subset->ID);
    }

    //Delete cloud
    delete database.cloud_selected;

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
    string log = "Cloud "+ name +" removed";
    console.AddLog("#", log);
  }
  if(database.list_cloud->size() == 0){
    Glyphs glyphManager;
    glyphManager.reset();
    database.cloud_selected = nullptr;
  }

  //---------------------------
}
void Scene::remove_cloud_all(){
  //---------------------------

  while(database.list_cloud->size() != 0){
    Cloud* cloud = *database.list_cloud->begin();
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

  /*
  //Delete subset iterator in cloud
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = get_subset(cloud, i);

    if(subset->ID == ID){
      //Supress subset objects
      Subset* subset_buf = get_subset_buffer(cloud, i);
      Subset* subset_ini = get_subset_init(cloud, i);

      delete subset;
      delete subset_buf;
      delete subset_ini;

      //Supress subset iterators
      list<Subset*>::iterator it = next(cloud->subset.begin(), i);
      list<Subset*>::iterator it_buf = next(cloud->subset_buffer.begin(), i);
      list<Subset*>::iterator it_ini = next(cloud->subset_init.begin(), i);

      cloud->subset.erase(it);
      cloud->subset_buffer.erase(it_buf);
      cloud->subset_init.erase(it_ini);

      break;
    }
  }
  */

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

  delete subset;
  delete subset_buf;
  delete subset_ini;

  //Supress subset iterators
  list<Subset*>::iterator it = next(cloud->subset.begin(), 0);
  list<Subset*>::iterator it_buf = next(cloud->subset_buffer.begin(), 0);
  list<Subset*>::iterator it_ini = next(cloud->subset_init.begin(), 0);

  cloud->subset.erase(it);
  cloud->subset_buffer.erase(it_buf);
  cloud->subset_init.erase(it_ini);

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

  //---------------------------
}
void Scene::add_subset_to_gpu(Subset* subset){
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
sayHello();
    //Delete obstacles glyphs
    for(int i=0; i<subset->obstacle_pr.oobb.size(); i++){
      say(subset->obstacle_pr.oobb.size());
      Glyph* glyph = subset->obstacle_pr.oobb[i];
      say(glyph->name);
      delete glyph;
    }
    subset->obstacle_pr.oobb.clear();
    for(int i=0; i<subset->obstacle_gt.oobb.size(); i++){
      Glyph* glyph = subset->obstacle_gt.oobb[i];
      delete glyph;
    }
    subset->obstacle_gt.oobb.clear();
sayHello();
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
  for(int i=0; i<database.list_cloud->size(); i++){
    Cloud* cloud = *next(database.list_cloud->begin(),i);
    this->reset_cloud(cloud);
  }

  this->update_cloud_glyphs(database.cloud_selected);

  //---------------------------
  console.AddLog("#", "Reset scene...");
}

//Updating - cloud
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

//Assesseurs
Cloud* Scene::get_cloud_selected(){
  //---------------------------

  if(database.cloud_selected != nullptr){
    return database.cloud_selected;
  }else{
    return nullptr;
  }

  //---------------------------
}
Cloud* Scene::get_cloud_next(){
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
Cloud* Scene::get_cloud_byName(string name){
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
Cloud* Scene::get_cloud_byoID(int oID){
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

Subset* Scene::get_subset_selected(){
  Cloud* cloud = database.cloud_selected;
  //---------------------------

  if(cloud != nullptr){
    Subset* subset = get_subset_byID(cloud, cloud->ID_selected);
    return subset;
  }
  else{
    return nullptr;
  }

  //---------------------------
}
Subset* Scene::get_subset_selected_init(){
  Cloud* cloud = database.cloud_selected;
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
      list_out->push_back(get_cloud_byName(nameByOrder[i]));
    }

    database.list_cloud = list_out;
  }

  //---------------------------
  return nameByOrder;
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

//Check state functions
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
