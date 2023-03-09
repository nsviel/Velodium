#include "Extractor.h"

#include "../Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Scene/Data/Scene.h"
#include "../../Engine/Core/Configuration.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/GPU/GPU_data.h"
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

  this->objectManager = node_scene->get_objectManager();
  this->texManager = new Texture();
  this->gpuManager = new GPU_data();

  this->ID_cloud = 0;
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
    gpuManager->gen_object_buffers(subset);
    this->define_visibility(subset, i);
    this->define_buffer_init(cloud, subset);
    this->compute_texture(subset, data[i]);

    cloud->ID_subset++;
  }

  //---------------------------
  return cloud;
}
Subset* Extractor::extract_data(Data_file& data){
  Subset* subset = new Subset();
  //---------------------------

  //Init
  this->check_data(&data);
  this->init_random_color();
  this->init_subset_parameter(subset, &data, 0);

  //Set parametrization
  objectManager->create_glyph_subset(subset);
  gpuManager->gen_object_buffers(subset);

  //---------------------------
  return subset;
}
void Extractor::extract_data(Cloud* cloud, Data_file* data){
  Subset* subset = new Subset();
  //---------------------------

  //Init
  this->color_rdm = cloud->unicolor;
  this->check_data(data);
  this->init_subset_parameter(subset, data, cloud->ID_subset);

  //Create associated glyphs
  objectManager->create_glyph_subset(subset);
  this->define_buffer_init(cloud, subset);
  gpuManager->gen_object_buffers(subset);

  //Update cloud stats
  cloud->nb_subset++;
  cloud->ID_subset++;

  //---------------------------
}

// Init function
void Extractor::check_data(Data_file* data){
  this->is_color = false;
  this->is_normal = false;
  this->is_intensity = false;
  this->is_timestamp = false;
  this->is_texture = false;
  //---------------------------

  //Normals
  if(data->Nxyz.size() != 0 && data->Nxyz.size() == data->xyz.size()){
    this->is_normal = true;
  }

  //Intensities
  if(data->I.size() != 0 && data->I.size() == data->xyz.size()){
    this->is_intensity = true;
  }
  if(fct_max(data->I) > 1){
    for(int i=0; i<data->I.size(); i++){
      data->I[i] = data->I[i] / 255;
    }
  }

  //Timestamp
  if(data->ts.size() != 0 && data->ts.size() == data->xyz.size()){
    this->is_timestamp = true;
  }

  //Texture
  if(data->uv.size() != 0){
    this->is_texture = true;
  }

  //---> if color data
  if(data->rgb.size() != 0){
    this->is_color = true;
  }
  //---> if intensity data
  else if(data->rgb.size() == 0 && data->I.size() != 0){
    for(int i=0; i<data->I.size(); i++){
      data->rgb.push_back(vec4(data->I.at(i), data->I.at(i), data->I.at(i), 1.0f));
    }
  }
  //---> if no color or intensity data
  else if(data->uv.size() == 0){
    for(int i=0; i<data->xyz.size(); i++){
      data->rgb.push_back(color_rdm);
    }
  }else if(data->uv.size() != 0){
    for(int i=0; i<data->xyz.size(); i++){
      data->rgb.push_back(vec4(1, 1, 1, 1));
    }
  }

  //---------------------------
}
void Extractor::init_cloud_parameter(Cloud* cloud, vector<Data_file*> data){
  //---------------------------

  //Calculate number of point
  int nb_point = 0;
  for(int i=0; i<data.size(); i++){
    nb_point += data[i]->xyz.size();
  }

  //General information
  string path = data[0]->path_file;
  cloud->path_file = path;
  cloud->name = get_name_from_path(path);
  cloud->file_format = get_format_from_path(path);

  cloud->is_visible = true;
  cloud->nb_point = nb_point;
  cloud->nb_subset = data.size();

  cloud->unicolor = color_rdm;
  cloud->path_save = get_path_abs_build() + "../media/data/";

  //---------------------------
}
void Extractor::init_subset_parameter(Subset* subset, Data_file* data, int ID){
  //---------------------------

  subset->xyz = data->xyz;
  subset->rgb = data->rgb;
  subset->Nxyz = data->Nxyz;
  subset->I = data->I;
  subset->ts = data->ts;
  subset->uv = data->uv;

  subset->draw_type_name = data->draw_type_name ;
  subset->unicolor = color_rdm;
  subset->has_color = is_color;
  subset->has_intensity = is_intensity;
  subset->has_normal = is_normal;
  subset->has_timestamp = is_timestamp;

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

// Param function
void Extractor::define_visibility(Subset* subset, int i){
  //---------------------------

  if(i == 0){
    subset->is_visible = true;
  }else{
    subset->is_visible = false;
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
void Extractor::compute_texture(Subset* subset, Data_file* data){
  if(data->path_texture == ""){return;}
  //---------------------------

  string name = get_name_from_path(data->path_texture);
  int ID = texManager->load_texture(data->path_texture, name);
  subset->tex_ID.push_back(ID);
  subset->has_texture = true;

  //---------------------------
}
