#include "Extractor.h"

#include "../../Scene/Glyph/Object.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/GPU/GPU_data.h"
#include "../../Specific/Function/fct_math.h"
#include "../../Specific/color.h"

#include "../../Specific/File/Directory.h"
#include "../../Specific/File/Info.h"
#include "../../Specific/File/Path.h"
#include "../../Operation/Color/Color.h"


// Constructor / Destructor
Extractor::Extractor(){
  //---------------------------

  this->objectManager = new Object();
  this->texManager = new Texture();
  this->gpuManager = new GPU_data();

  this->ID_cloud = 0;
  this->ID = 0;

  //---------------------------
}
Extractor::~Extractor(){}

// Main function
Collection* Extractor::extract_data(vector<Data_file*> data){
  Collection* collection = new Collection();
  //---------------------------

  if(data.size() == 0){
    cout<<"[error] Data size equal zero"<<endl;
    exit(0);
  }

  //Init collection parameters
  this->init_random_color();
  this->init_collection_parameter(collection, data);

  for(int i=0; i<data.size(); i++){
    Cloud* cloud = new Cloud();

    //Init
    this->check_data(data[i]);
    this->init_cloud_parameter(cloud, data[i], collection->ID_obj_last);
    objectManager->create_glyph_object(cloud);

    //Set parametrization
    gpuManager->gen_object_buffers(cloud);
    this->define_visibility(cloud, i);
    collection->obj_add_new(cloud);
    this->compute_texture(cloud, data[i]);

    collection->ID_obj_last++;
  }

  //---------------------------
  return collection;
}
Cloud* Extractor::extract_data(Data_file& data_file){
  Cloud* object = new Cloud();
  //---------------------------

  //Init
  this->check_data(&data_file);
  this->init_random_color();
  this->init_cloud_parameter(object, &data_file, 0);

  //Set parametrization
  objectManager->create_glyph_object(object);
  gpuManager->gen_object_buffers(object);

  //---------------------------
  return object;
}
Object_* Extractor::extract_data_object(Data_file* data_file){
  Object_* object = new Object_();
  //---------------------------

  //Init
  this->check_data(data_file);
  this->init_random_color();
  this->init_object_parameter(object, data_file, 0);

  //Set parametrization
  gpuManager->gen_object_buffers(object);

  //---------------------------
  return object;
}
void Extractor::extract_data(Collection* collection, Data_file* data){
  Cloud* cloud = new Cloud();
  //---------------------------

  //Init
  this->color_rdm = collection->unicolor;
  this->check_data(data);
  this->init_cloud_parameter(cloud, data, collection->ID_obj_last);

  //Create associated glyphs
  objectManager->create_glyph_object(cloud);
  gpuManager->gen_object_buffers(cloud);

  //Update collection stats
  collection->obj_add_new(cloud);
  collection->nb_obj++;
  collection->ID_obj_last++;
  cloud->ID = collection->ID_obj_otf++;

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
void Extractor::init_collection_parameter(Collection* collection, vector<Data_file*> data){
  //---------------------------

  //Calculate number of point
  int nb_point = 0;
  for(int i=0; i<data.size(); i++){
    nb_point += data[i]->xyz.size();
  }

  //General information
  string path = data[0]->path_file;
  collection->path_file_load = path;
  collection->name = get_name_from_path(path);
  collection->file_format = get_format_from_path(path);

  collection->is_visible = true;
  collection->nb_obj = data.size();

  collection->unicolor = color_rdm;
  collection->path_file_save = get_path_abs_build() + "../media/data/";

  //---------------------------
}
void Extractor::init_object_parameter(Object_* object, Data_file* data, int ID){
  //---------------------------

  object->xyz = data->xyz;
  object->rgb = data->rgb;
  object->Nxyz = data->Nxyz;
  object->uv = data->uv;

  object->draw_type_name = data->draw_type_name ;
  object->unicolor = color_rdm;
  object->has_color = is_color;
  object->has_normal = is_normal;

  gpuManager->gen_vao(object);

  // Subset info
  object->ID = ID;
  if(data->name != ""){
    object->name = data->name;
  }else{
    object->name = "frame_" + to_string(ID);
  }

  //---------------------------
}
void Extractor::init_cloud_parameter(Cloud* cloud, Data_file* data, int ID){
  //---------------------------

  cloud->xyz = data->xyz;
  cloud->rgb = data->rgb;
  cloud->Nxyz = data->Nxyz;
  cloud->I = data->I;
  cloud->ts = data->ts;
  cloud->uv = data->uv;

  cloud->draw_type_name = data->draw_type_name ;
  cloud->unicolor = color_rdm;
  cloud->has_color = is_color;
  cloud->has_intensity = is_intensity;
  cloud->has_normal = is_normal;
  cloud->has_timestamp = is_timestamp;

  gpuManager->gen_vao(cloud);

  // Subset info
  cloud->ID = ID;
  if(data->name != ""){
    cloud->name = data->name;
  }else{
    cloud->name = "frame_" + to_string(ID);
  }

  // Structure
  cloud->frame.reset();

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
void Extractor::define_visibility(Cloud* cloud, int i){
  //---------------------------

  if(i == 0){
    cloud->is_visible = true;
  }else{
    cloud->is_visible = false;
  }

  //---------------------------
}
void Extractor::compute_texture(Cloud* cloud, Data_file* data){
  if(data->path_texture == ""){return;}
  //---------------------------

  string name = get_name_from_path(data->path_texture);
  int ID = texManager->load_texture(data->path_texture, name);
  cloud->tex_ID.push_back(ID);
  cloud->has_texture = true;

  //---------------------------
}
