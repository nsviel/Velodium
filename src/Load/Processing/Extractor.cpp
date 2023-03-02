#include "Extractor.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/OpenGL/GPU_transfert.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/color.h"
#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Info.h"
#include "../../Specific/File/Path.h"
#include "../../Operation/Color/Color.h"


// Constructor / Destructor
Extractor::Extractor(Node_load* node_load){
  //---------------------------

  Node_engine* node_engine = node_load->get_node_engine();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->gpuManager = new GPU_transfert();

  this->configManager = node_engine->get_configManager();
  this->sceneManager = node_scene->get_sceneManager();
  this->objectManager = node_scene->get_objectManager();
  this->texManager = node_engine->get_texManager();

  this->ID = 0;

  //---------------------------
}
Extractor::~Extractor(){}

// Main function
Cloud* Extractor::extract_data(vector<Data_file*> data){
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

    //Init
    this->check_data(data[i]);
    this->init_subset_parameter(subset, data[i], cloud->ID_subset);
    objectManager->create_glyph_subset(subset);

    //Set parametrization
    this->insert_subset_into_gpu(subset);
    this->define_visibility(subset, i);
    this->define_buffer_init(cloud, subset);
    this->compute_texture(subset, data[i]);

    cloud->ID_subset++;
  }

  //---------------------------
  return cloud;
}
Subset* Extractor::extract_data(Data_cap& data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_random_color();
  this->check_data(data);

  this->init_subset_parameter(subset, data.name, 0);

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
void Extractor::insert_subset_into_gpu(Subset* subset){
  //---------------------------

  gpuManager->convert_draw_type_byName(subset);
  gpuManager->bind_buffer_texture(subset);
  gpuManager->bind_buffer_location(subset);
  gpuManager->bind_buffer_color(subset);

  //---------------------------
}
void Extractor::extract_data_frame(Cloud* cloud, Data_file* data){
  Subset* subset = new Subset();
  //---------------------------

  this->color_rdm = cloud->unicolor;
  this->check_data(data);

  this->init_subset_parameter(subset, data->name, cloud->ID_subset);

  //Subset data
  this->extract_location(subset, data->location);
  this->extract_intensity(subset, data->intensity);
  this->extract_color(subset, data->color);
  this->extract_normal(subset, data->normal);
  this->extract_timestamp(subset, data->timestamp);

  //Create associated glyphs
  objectManager->create_glyph_subset(subset);
  this->define_buffer_init(cloud, subset);

  cloud->nb_subset++;
  cloud->ID_subset++;

  //---------------------------
}
void Extractor::extract_data_oneFrame(Cloud* cloud, Data_file* data){
  Subset* subset = new Subset();
  //---------------------------

  this->init_random_color();
  this->check_data(data);

  uint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  subset->vao = VAO;
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

// Subfunctions
void Extractor::check_data(Data_file* data){
  this->is_color = false;
  this->is_normal = false;
  this->is_intensity = false;
  this->is_timestamp = false;
  this->is_texture = false;
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

  //Texture
  if(data->texture.size() != 0){
    this->is_texture = true;
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
  else if(data->texture.size() == 0){
    for(int i=0; i<data->location.size(); i++){
      data->color.push_back(color_rdm);
    }
  }else if(data->texture.size() != 0){
    for(int i=0; i<data->location.size(); i++){
      data->color.push_back(vec4(1, 1, 1, 1));
    }
  }

  //---------------------------
}
void Extractor::check_data(Data_cap& data){
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
void Extractor::define_visibility(Subset* subset, int i){
  //---------------------------

  if(i == 0){
    subset->visibility = true;
  }else{
    subset->visibility = false;
  }

  //---------------------------
}
void Extractor::define_buffer_init(Cloud* cloud, Subset* subset){
  //---------------------------

  Subset* subset_buf = new Subset(*subset);
  Subset* subset_ini = new Subset(*subset);

  cloud->subset_selected = subset;
  cloud->subset.push_back(subset);
  cloud->subset_buffer.push_back(subset_buf);
  cloud->subset_init.push_back(subset_ini);

  //---------------------------
}
void Extractor::init_cloud_parameter(Cloud* cloud, vector<Data_file*> data){
  //---------------------------

  //Calculate number of point
  int nb_point = 0;
  for(int i=0; i<data.size(); i++){
    nb_point += data[i]->location.size();
  }

  //General information
  string path = data[0]->path;
  cloud->path = path;
  cloud->name = get_name_from_path(path);
  cloud->format = get_format_from_path(path);

  cloud->visibility = true;
  cloud->nb_point = nb_point;
  cloud->nb_subset = data.size();

  cloud->point_size = configManager->parse_json_i("parameter", "point_size");
  cloud->unicolor = color_rdm;
  cloud->saveas = get_path_abs_build() + "../media/data/";

  //ID
  int* ID_cloud = sceneManager->get_new_ID_cloud();
  cloud->ID = *ID_cloud;
  cloud->oID = sceneManager->get_new_oID_cloud();
  *ID_cloud += 1;

  //---------------------------
}
void Extractor::init_subset_parameter(Subset* subset, string name, int ID){
  //---------------------------

  gpuManager->gen_vao(subset);

  // Subset info
  subset->ID = ID;
  if(name != ""){
    subset->name = name;
  }else{
    subset->name = "frame_" + to_string(ID);
  }

  // Structure
  subset->frame.reset();

  //---------------------------
}
void Extractor::init_subset_parameter(Subset* subset, Data_file* data, int ID){
  //---------------------------

  subset->xyz = data->location;
  subset->rgb = data->color;
  subset->Nxyz = data->normal;
  subset->I = data->intensity;
  subset->ts = data->timestamp;
  subset->uv = data->texture;

  subset->draw_type_name = data->draw_type_name ;
  subset->unicolor = color_rdm;
  subset->has_color = is_color;
  subset->has_intensity = is_intensity;
  subset->has_normal = is_normal;
  subset->has_timestamp = is_timestamp;
  subset->has_texture = is_texture;

  gpuManager->gen_vao(subset);

  // Subset info
  subset->ID = ID;
  if(data->name != ""){
    subset->name = data->name;
  }else{
    subset->name = "frame_" + to_string(ID);
  }

  // Structure
  subset->frame.reset();

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

// Data type extraction
void Extractor::extract_location(Subset* subset, vector<vec3>& locationOBJ){
  uint positionVBO;
  //---------------------------

  //Get data
  subset->nb_point = locationOBJ.size();
  subset->xyz = locationOBJ;

  //Transformation matrices
  subset->scale = mat4(1.0);
  subset->trans = mat4(1.0);
  subset->rotat = mat4(1.0);
  subset->transformation = mat4(1.0);

  //Bind data
  gpuManager->bind_buffer_location(subset);

  //---------------------------
}
void Extractor::extract_intensity(Subset* subset, vector<float>& vec_I){
  //---------------------------

  if(is_intensity){
    subset->I = vec_I;
    subset->has_intensity = true;
  }

  //---------------------------
}
void Extractor::extract_timestamp(Subset* subset, vector<float>& vec_ts){
  //---------------------------

  if(is_timestamp){
    subset->ts = vec_ts;
    subset->has_timestamp = true;
  }
  else{
    //If not timestamp, set all timestamp at 0
    vector<float> ts (subset->xyz.size(), 0);
    subset->ts = ts;
  }

  //---------------------------
}
void Extractor::extract_normal(Subset* subset, vector<vec3>& vec_Nxyz){
  uint normalVBO;
  //---------------------------

  if(is_normal){
    subset->Nxyz = vec_Nxyz;
    subset->has_normal = true;
  }

  //---------------------------
}
void Extractor::extract_color(Subset* subset, vector<vec4>& vec_rgb){
  //---------------------------

  subset->rgb = vec_rgb;
  subset->unicolor = color_rdm;

  if(is_color){
    subset->has_color = true;
  }

  //Bind data
  gpuManager->bind_buffer_color(subset);

  //---------------------------
}
void Extractor::extract_texture(Subset* subset, vector<vec2>& vec_tex){
  //---------------------------

  subset->uv = vec_tex;

  //Bind data
  gpuManager->bind_buffer_texture(subset);

  //---------------------------
}

// Texture
void Extractor::compute_texture(Subset* subset, Data_file* data){
  if(data->path_texture == ""){return;}
  //---------------------------

  string name = get_name_from_path(data->path_texture);
  int ID = texManager->load_texture(data->path_texture, name);
  subset->texture_ID = ID;
  subset->has_texture = true;

  //---------------------------
}
