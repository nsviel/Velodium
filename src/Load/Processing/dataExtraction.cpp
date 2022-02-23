#include "dataExtraction.h"

#include "../../Engine/Scene/Glyphs.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Specific/fct_maths.h"
#include "../../Specific/fct_system.h"


//Constructor / Destructor
dataExtraction::dataExtraction(){
  //---------------------------

  this->configManager = new Configuration();
  this->sceneManager = new Scene();

  this->ID = 0;

  //---------------------------
}
dataExtraction::~dataExtraction(){}

//Main function
Cloud* dataExtraction::extractData(vector<dataFile*> data){
  Cloud* cloud = new Cloud();
  //---------------------------

  //Init cloud parameters
  this->init_randomColor();
  this->init_cloudParameters(cloud, data);

  for(int i=0; i<data.size(); i++){
    Subset* subset = new Subset();

    this->check_data(data[i]);
    this->init_subsetParameters(subset, data[i]->name, cloud->ID_subset);
    this->init_frameParameters(subset);
    cloud->ID_subset++;

    //Subset data
    this->extract_Location(subset, data[i]->location);
    this->extract_Intensity(subset, data[i]->intensity);
    this->extract_Color(subset, data[i]->color);
    this->extract_Normal(subset, data[i]->normal);
    this->extract_Timestamp(subset, data[i]->timestamp);

    if(i == 0){
      subset->visibility = true;
    }else{
      subset->visibility = false;
    }

    //Create associated glyphs
    Glyphs glyphManager;
    glyphManager.create_glyph_fromCloud(subset);

    Subset* subset_buf = new Subset(*subset);
    Subset* subset_ini = new Subset(*subset);

    cloud->subset.push_back(subset);
    cloud->subset_buffer.push_back(subset_buf);
    cloud->subset_init.push_back(subset_ini);
  }

  //---------------------------
  return cloud;
}
Subset* dataExtraction::extractData(udpPacket& data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_randomColor();
  this->check_data(data);

  this->init_subsetParameters(subset, data.name, 0);
  this->init_frameParameters(subset);

  //Subset data
  this->extract_Location(subset, data.xyz);
  this->extract_Intensity(subset, data.I);
  this->extract_Timestamp(subset, data.t);
  this->extract_Color(subset, data.rgb);

  //Create associated glyphs
  Glyphs glyphManager;
  glyphManager.create_glyph_fromCloud(subset);

  //---------------------------
  return subset;
}
void dataExtraction::extractData_frame(Cloud* cloud, dataFile* data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_randomColor();
  this->check_data(data);

  this->init_subsetParameters(subset, data->name, cloud->ID_subset);
  this->init_frameParameters(subset);

  //Subset data
  this->extract_Location(subset, data->location);
  this->extract_Intensity(subset, data->intensity);
  this->extract_Color(subset, data->color);
  this->extract_Normal(subset, data->normal);
  this->extract_Timestamp(subset, data->timestamp);

  //Create associated glyphs
  Glyphs glyphManager;
  glyphManager.create_glyph_fromCloud(subset);

  cloud->subset.push_back(subset);
  cloud->subset_init.push_back(subset);
  cloud->subset_buffer.push_back(subset);
  cloud->nb_subset++;
  cloud->ID_subset++;

  //---------------------------
}
void dataExtraction::extractData_oneFrame(Cloud* cloud, dataFile* data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_randomColor();
  this->check_data(data);

  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  subset->VAO = VAO;
  subset->name = "oneFrame";
  subset->visibility = true;

  //Subset data
  this->extract_Location(subset, data->location);
  this->extract_Intensity(subset, data->intensity);
  this->extract_Color(subset, data->color);
  this->extract_Normal(subset, data->normal);
  this->extract_Timestamp(subset, data->timestamp);

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
void dataExtraction::check_data(dataFile* data){
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

  //Name
  string path = data->path;
  if(data->name == ""){
    string name = path.substr(path.find_last_of("/\\") + 1);
    name =  name.substr(0, name.find_last_of("/"));
    data->name = name.substr(0, name.find_last_of("."));
  }

  //---------------------------
}
void dataExtraction::check_data(udpPacket& data){
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
void dataExtraction::init_cloudParameters(Cloud* cloud, vector<dataFile*> data){
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
  cloud->heatmap = false;
  cloud->point_size = configManager->parse_json_i("parameter", "point_size");
  cloud->unicolor = color_rdm;
  cloud->saveas = get_absolutePath_build() + "/../media/data/";

  //ID
  int* list_ID = sceneManager->get_list_ID_cloud();
  cloud->ID = *list_ID;
  cloud->oID = *list_ID;
  *list_ID++;

  //---------------------------
}
void dataExtraction::init_subsetParameters(Subset* subset, string name, int ID){
  //---------------------------

  //Subset VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  subset->VAO = VAO;

  //Other stuff
  subset->ID = ID;
  subset->name = name;
  subset->root = vec3(0.0);

  //---------------------------
}
void dataExtraction::init_frameParameters(Subset* subset){
  Frame* frame = &subset->frame;
  //---------------------------

  frame->reset();

  //---------------------------
}
void dataExtraction::init_randomColor(){
  //---------------------------

  //---> Compute a random color for each cloud
  color_rdm = random_color();

  //First cloud color
  if(ID == 0){
    color_rdm.x = 0.08;
    color_rdm.y = 0.3;
    color_rdm.z = 0.44;
    ID++;
  }

  //---------------------------
}

void dataExtraction::extract_Location(Subset* subset, vector<vec3>& locationOBJ){
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
void dataExtraction::extract_Intensity(Subset* subset, vector<float>& intensityOBJ){
  //---------------------------

  if(is_intensity){
    subset->I = intensityOBJ;
  }

  //---------------------------
}
void dataExtraction::extract_Timestamp(Subset* subset, vector<float>& timestampOBJ){
  //---------------------------

  if(is_timestamp){
    subset->ts = timestampOBJ;
  }
  else{
    vector<float> ts (subset->xyz.size(), 0);
    subset->ts = ts;
  }

  //---------------------------
}
void dataExtraction::extract_Normal(Subset* subset, vector<vec3>& normalOBJ){
  uint normalVBO;
  //---------------------------

  if(is_normal){
    subset->N = normalOBJ;
  }

  //---------------------------
}
void dataExtraction::extract_Color(Subset* subset, vector<vec4>& colorOBJ){
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
