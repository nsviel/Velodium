#include "Extractor.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
Extractor::Extractor(Node_load* node_load){
  //---------------------------

  Node_engine* node_engine = node_load->get_node_engine();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->ID = 0;

  //---------------------------
}
Extractor::~Extractor(){}

//Main function
Cloud* Extractor::extract_data(vector<dataFile*> data){
  Cloud* cloud = new Cloud();
  //---------------------------

  if(data.size() == 0){
    cout<<"[error] Data size equal zero"<<endl;
    exit(0);
  }

  //Init cloud parameters
  this->init_random_color();
  this->init_cloud_parameter(cloud, data);

  for(int i=0; i<data.size(); i++){
    Subset* subset = new Subset();

    this->check_data(data[i]);
    this->init_subset_parameter(subset, data[i]->name, cloud->ID_subset);
    this->init_frame_parameter(subset);
    cloud->ID_subset++;

    //Subset data
    this->extract_location(subset, data[i]->location);
    this->extract_intensity(subset, data[i]->intensity);
    this->extract_color(subset, data[i]->color);
    this->extract_normal(subset, data[i]->normal);
    this->extract_timestamp(subset, data[i]->timestamp);

    if(i == 0){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }

    //Create associated glyphs
    objectManager->create_glyph_subset(subset);

    Subset* subset_buf = new Subset(*subset);
    Subset* subset_ini = new Subset(*subset);

    cloud->subset_selected = subset;
    cloud->subset.push_back(subset);
    cloud->subset_buffer.push_back(subset_buf);
    cloud->subset_init.push_back(subset_ini);
  }

  //---------------------------
  return cloud;
}
Subset* Extractor::extract_data(udpPacket& data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_random_color();
  this->check_data(data);

  this->init_subset_parameter(subset, data.name, 0);
  this->init_frame_parameter(subset);

  //Subset data
  this->extract_location(subset, data.xyz);
  this->extract_intensity(subset, data.I);
  this->extract_timestamp(subset, data.t);
  this->extract_color(subset, data.rgb);

  //Create associated glyphs
  objectManager->create_glyph_subset(subset);

  //---------------------------
  return subset;
}
void Extractor::extract_data_frame(Cloud* cloud, dataFile* data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_random_color();
  this->check_data(data);

  this->init_subset_parameter(subset, data->name, cloud->ID_subset);
  this->init_frame_parameter(subset);

  //Subset data
  this->extract_location(subset, data->location);
  this->extract_intensity(subset, data->intensity);
  this->extract_color(subset, data->color);
  this->extract_normal(subset, data->normal);
  this->extract_timestamp(subset, data->timestamp);

  //Create associated glyphs
  objectManager->create_glyph_subset(subset);

  cloud->subset.push_back(subset);
  cloud->subset_init.push_back(subset);
  cloud->subset_buffer.push_back(subset);
  cloud->nb_subset++;
  cloud->ID_subset++;

  //---------------------------
}
void Extractor::extract_data_oneFrame(Cloud* cloud, dataFile* data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_random_color();
  this->check_data(data);

  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  subset->VAO = VAO;
  subset->name = "oneFrame";
  subset->visibility = true;

  //Subset data
  this->extract_location(subset, data->location);
  this->extract_intensity(subset, data->intensity);
  this->extract_color(subset, data->color);
  this->extract_normal(subset, data->normal);
  this->extract_timestamp(subset, data->timestamp);

  if(cloud->subset.size() == 0){
    cloud->subset.push_back(subset);
    cloud->subset_buffer.push_back(subset);
    cloud->subset_init.push_back(subset);

    cloud->nb_subset = 1;
  }else{
    Subset* subset_m0 = sceneManager->get_subset(cloud, 0);
    Subset* subset_buffer_m0 = sceneManager->get_subset_buffer(cloud, 0);
    Subset* subset_init_m0 = sceneManager->get_subset_init(cloud, 0);

    subset_m0 = subset;
    subset_buffer_m0 = subset;
    subset_init_m0 = subset;

    cloud->nb_subset = 1;
  }

  //---------------------------
}

//Subfunctions
void Extractor::check_data(dataFile* data){
  this->is_color = false;
  this->is_normal = false;
  this->is_intensity = false;
  this->is_timestamp = false;
  //---------------------------

  //Normals
  if(data->normal.size() != 0 && data->normal.size() == data->location.size()){
    this->is_normal = true;
  }

  //Intensities
  if(data->intensity.size() != 0 && data->intensity.size() == data->location.size()){
    this->is_intensity = true;
  }

  //Timestamp
  if(data->timestamp.size() != 0 && data->timestamp.size() == data->location.size()){
    this->is_timestamp = true;
  }

  //---> if color data
  if(data->color.size() != 0){
    this->is_color = true;
  }
  //---> if intensity data
  else if(data->color.size() == 0 && data->intensity.size() != 0){
    for(int i=0; i<data->intensity.size(); i++){
      data->color.push_back(vec4(data->intensity.at(i), data->intensity.at(i), data->intensity.at(i), 1.0f));
    }
  }
  //---> if no color or intensity data
  else{
    for(int i=0; i<data->location.size(); i++){
      data->color.push_back(color_rdm);
    }
  }

  //---------------------------
}
void Extractor::check_data(udpPacket& data){
  this->is_color = false;
  this->is_normal = false;
  this->is_intensity = false;
  this->is_timestamp = false;
  //---------------------------

  //Intensities
  if(data.I.size() != 0 && data.I.size() == data.xyz.size()){
    this->is_intensity = true;
  }
  if(fct_max(data.I) > 1){
    for(int i=0; i<data.I.size(); i++){
      data.I[i] = data.I[i] / 255;
    }
  }

  //Timestamp
  if(data.t.size() != 0 && data.t.size() == data.xyz.size()){
    this->is_timestamp = true;
  }

  //Color
  if(is_intensity){
    for(int i=0; i<data.I.size(); i++){
      data.rgb.push_back(vec4(data.I.at(i), data.I.at(i), data.I.at(i), 1.0f));
    }
  }

  //---------------------------
}
void Extractor::init_cloud_parameter(Cloud* cloud, vector<dataFile*> data){
  //---------------------------

  //Calculate number of point
  int nb_point = 0;
  for(int i=0; i<data.size(); i++){
    nb_point += data[i]->location.size();
  }

  //General information
  string filePath = data[0]->path;
  if(filePath != ""){
    string nameFormat = filePath.substr(filePath.find_last_of("/\\") + 1);
    cloud->path =  nameFormat.substr(0, nameFormat.find_last_of("/"));
    cloud->name = nameFormat.substr(0, nameFormat.find_last_of("."));
    cloud->format = nameFormat.substr(nameFormat.find_last_of("."), string::npos);
  }else{
    cloud->path = "";
    cloud->name = "";
    cloud->format = "";
  }

  cloud->dataFormat = "";
  cloud->visibility = true;
  cloud->nb_point = nb_point;
  cloud->nb_subset = data.size();
  cloud->ID_selected = 0;
  cloud->ID_subset = 0;
  cloud->ID_file = 0;
  cloud->heatmap = false;
  cloud->onthefly = false;
  cloud->lidar_model = "";
  cloud->point_size = configManager->parse_json_i("parameter", "point_size");
  cloud->unicolor = color_rdm;
  cloud->saveas = get_absolutePath_build() + "../media/data/";

  //ID
  int* ID_cloud = sceneManager->get_new_ID_cloud();
  cloud->ID = *ID_cloud;
  cloud->oID = sceneManager->get_new_oID_cloud();
  *ID_cloud += 1;

  //---------------------------
}
void Extractor::init_subset_parameter(Subset* subset, string name, int ID){
  //---------------------------

  //Subset VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  subset->VAO = VAO;

  //Other stuff
  subset->ID = ID;
  subset->root = vec3(0.0);
  if(name != ""){
    subset->name = name;
  }else{
    subset->name = "frame_" + to_string(ID);
  }

  //---------------------------
}
void Extractor::init_frame_parameter(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  frame->reset();

  //---------------------------
}
void Extractor::init_random_color(){
  //---------------------------

  //---> Compute a random color for each cloud
  color_rdm = random_color();

  //First cloud color
  if(ID == 0){
    color_rdm.x = (float) 175/255;
    color_rdm.y = (float) 175/255;
    color_rdm.z = (float) 175/255;
    ID++;
  }

  //---------------------------
}

//Data type extraction
void Extractor::extract_location(Subset* subset, vector<vec3>& locationOBJ){
  uint positionVBO;
  //---------------------------

  glGenBuffers(1, &positionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  glBufferData(GL_ARRAY_BUFFER, locationOBJ.size()*sizeof(glm::vec3), &locationOBJ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  subset->nb_point = locationOBJ.size();
  subset->VBO_xyz = positionVBO;
  subset->xyz = locationOBJ;

  //Transformation matrices
  subset->scale = mat4(1.0);
  subset->trans = mat4(1.0);
  subset->rotat = mat4(1.0);
  subset->transformation = mat4(1.0);

  //---------------------------
}
void Extractor::extract_intensity(Subset* subset, vector<float>& intensityOBJ){
  //---------------------------

  if(is_intensity){
    subset->I = intensityOBJ;
  }

  //---------------------------
}
void Extractor::extract_timestamp(Subset* subset, vector<float>& timestampOBJ){
  //---------------------------

  if(is_timestamp){
    subset->ts = timestampOBJ;
    subset->has_timestamp = true;
  }
  else{
    //If not timestamp, set all timestamp at 0
    vector<float> ts (subset->xyz.size(), 0);
    subset->ts = ts;
    subset->has_timestamp = false;
  }

  //---------------------------
}
void Extractor::extract_normal(Subset* subset, vector<vec3>& normalOBJ){
  uint normalVBO;
  //---------------------------

  if(is_normal){
    subset->N = normalOBJ;
  }

  //---------------------------
}
void Extractor::extract_color(Subset* subset, vector<vec4>& colorOBJ){
  uint colorVBO;
  //---------------------------

  //Create OpenGL color object
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, colorOBJ.size()*sizeof(glm::vec4), &colorOBJ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  subset->VBO_rgb = colorVBO;
  subset->RGB = colorOBJ;
  subset->unicolor = color_rdm;

  if(is_color){
    subset->has_color = true;
  }

  //---------------------------
}
