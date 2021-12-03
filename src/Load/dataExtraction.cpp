#include "dataExtraction.h"

#include "../Engine/Data/Database.h"
#include "../Engine/Glyphs.h"

#include <experimental/filesystem>
#include <random>

extern struct Database database;

//Constructor / Destructor
dataExtraction::dataExtraction(){
  //---------------------------

  this->ID = 0;

  //---------------------------
}
dataExtraction::~dataExtraction(){}

//Main function
Cloud* dataExtraction::extractData(vector<dataFile*> data){
  Cloud* cloud = new Cloud();
  //---------------------------

  //Init cloud parameters
  this->init_cloudParameters(cloud, data);
  this->init_randomColor();

  for(int i=0; i<data.size(); i++){
    Subset subset;

    this->check_data(data[i]);
    this->init_subsetParameters(subset, data[i]->name, i);
    this->init_frameParameters(subset);

    //Subset data
    this->extract_Location(subset, data[i]->location);
    this->extract_Intensity(subset, data[i]->intensity);
    this->extract_Color(subset, data[i]->color);
    this->extract_Normal(subset, data[i]->normal);
    this->extract_Timestamp(subset, data[i]->timestamp);

    if(i == 0){
      subset.visibility = true;
    }else{
      subset.visibility = false;
    }

    //Create associated glyphs
    Glyphs glyphManager;
    glyphManager.create_glyph_fromCloud(&subset);

    cloud->subset.push_back(subset);
    cloud->subset_buffer.push_back(subset);
    cloud->subset_init.push_back(subset);
  }

  //---------------------------
  return cloud;
}
Subset dataExtraction::extractData(udpPacket* data){
  Subset subset;
  //---------------------------

  this->init_subsetParameters(subset, "frame", 0);
  this->init_frameParameters(subset);

  //Subset data
  this->extract_Location(subset, data->xyz);
  this->extract_Intensity(subset, data->I);
  this->extract_Timestamp(subset, data->t);

  //Create associated glyphs
  Glyphs glyphManager;
  glyphManager.create_glyph_fromCloud(&subset);

  //---------------------------
  return subset;
}
void dataExtraction::extractData_frame(Cloud* cloud, dataFile* data){
  Subset subset;
  //---------------------------

  this->check_data(data);
  this->init_subsetParameters(subset, data->name, cloud->nb_subset);
  this->init_frameParameters(subset);

  //Subset data
  this->extract_Location(subset, data->location);
  this->extract_Intensity(subset, data->intensity);
  this->extract_Color(subset, data->color);
  this->extract_Normal(subset, data->normal);
  this->extract_Timestamp(subset, data->timestamp);

  //Create associated glyphs
  Glyphs glyphManager;
  glyphManager.create_glyph_fromCloud(&subset);

  cloud->subset.push_back(subset);
  cloud->subset_init.push_back(subset);
  cloud->subset_buffer.push_back(subset);
  cloud->nb_subset++;

  //---------------------------
}
void dataExtraction::extractData_oneFrame(Cloud* cloud, dataFile* data){
  Subset subset;
  //---------------------------

  this->check_data(data);

  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  subset.VAO = VAO;
  subset.name = "oneFrame";
  subset.visibility = true;

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
    cloud->subset[0] = subset;
    cloud->subset_buffer[0] = subset;
    cloud->subset_init[0] = subset;
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

  //Color
  this->RGB_rdm = vec4(Red, Green, Blue, 1.0f);

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
      data->color.push_back(vec4(Red, Green, Blue, 1.0f));
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

  cloud->visibility = true;
  cloud->nb_point = nb_point;
  cloud->nb_subset = data.size();
  cloud->subset_selected = 0;
  cloud->heatmap = false;
  cloud->point_size = 1;

  //Save path
  string absPath = std::experimental::filesystem::current_path();
  cloud->saveas = absPath + "/../media/data/";

  //ID
  cloud->ID = database.ID_cloud;
  cloud->oID = database.ID_cloud;
  database.ID_cloud++;

  //---------------------------
}
void dataExtraction::init_subsetParameters(Subset& subset, string name, int idx){
  //---------------------------

  //Subset VAO
  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  subset.VAO = VAO;

  //Other stuff
  subset.name = name;
  subset.root = vec3(0.0);
  subset.dataFormat = " ";

  //---------------------------
}
void dataExtraction::init_frameParameters(Subset& subset){
  Frame* frame = &subset.frame;
  //---------------------------

  frame->time_slam = 0;

  frame->rotat_b = Eigen::Matrix3d::Identity();
  frame->rotat_e = Eigen::Matrix3d::Identity();
  frame->trans_b = Eigen::Vector3d::Zero();
  frame->trans_e = Eigen::Vector3d::Zero();

  //---------------------------
}
void dataExtraction::init_randomColor(){
  //---------------------------

  //---> Compute a random color for each cloud
  Red = float(rand()%101)/100;
  Green = float(rand()%101)/100;
  Blue = float(rand()%101)/100;

  //---------------------------
}

void dataExtraction::extract_Location(Subset& subset, vector<vec3>& locationOBJ){
  uint positionVBO;
  //---------------------------

  glGenBuffers(1, &positionVBO);
  glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
  glBufferData(GL_ARRAY_BUFFER, locationOBJ.size()*sizeof(glm::vec3), &locationOBJ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  subset.nb_point = locationOBJ.size();
  subset.VBO_xyz = positionVBO;
  subset.xyz = locationOBJ;

  //Transformation matrices
  subset.scale = mat4(1.0);
  subset.trans = mat4(1.0);
  subset.rotat = mat4(1.0);
  subset.transformation = mat4(1.0);

  //---------------------------
}
void dataExtraction::extract_Intensity(Subset& subset, vector<float>& intensityOBJ){
  //---------------------------

  if(is_intensity){
    subset.I = intensityOBJ;
  }

  //---------------------------
}
void dataExtraction::extract_Timestamp(Subset& subset, vector<float>& timestampOBJ){
  //---------------------------

  if(is_timestamp){
    subset.ts = timestampOBJ;
  }

  //---------------------------
}
void dataExtraction::extract_Normal(Subset& subset, vector<vec3>& normalOBJ){
  uint normalVBO;
  //---------------------------

  if(is_normal){
    subset.N = normalOBJ;
  }

  //---------------------------
}
void dataExtraction::extract_Color(Subset& subset, vector<vec4>& colorOBJ){
  uint colorVBO;
  //---------------------------

  //Create OpenGL color object
  glGenBuffers(1, &colorVBO);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, colorOBJ.size()*sizeof(glm::vec4), &colorOBJ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), 0);
  glEnableVertexAttribArray(1);

  subset.VBO_rgb = colorVBO;
  subset.RGB = colorOBJ;
  subset.unicolor = RGB_rdm;

  if(is_color){
    subset.has_color = true;
  }

  //---------------------------
}
