#include "file_PLY.h"

#include "../../Specific/Function/fct_math.h"

#include <iomanip>
#include <sys/file.h>


//Constructor / Destructor
file_PLY::file_PLY(){}
file_PLY::~file_PLY(){}

//Main loader functions
Data_file* file_PLY::Loader(string path_file){
  data_out = new Data_file();
  string nameFormat = path_file.substr(path_file.find_last_of("/\\") + 1);
  data_out->name = nameFormat.substr(0, nameFormat.find_last_of("."));
  data_out->path_file = path_file;
  this->face_number = 0;
  //---------------------------

  //Get format type
  std::ifstream file(path_file);
  this->Loader_header(file);

  //Open data
  if (format == "ascii"){

    //Open file
    std::ifstream file(path_file);

    //Read header
    this->Loader_header(file);

    //Read data
    if(face_number == 0){
      this->Loader_ascii(file);
    }else{
      this->Loader_ascii_withface(file);
    }

    file.close();

  }
  else if (format == "binary_little_endian"){
    //Open file
    std::ifstream file(path_file, ios::binary);

    //Read header
    this->Loader_header(file);

    //Read data
    if(face_number == 0){
      this->Loader_bin_little_endian(file);
    }else{
      this->Loader_bin_little_endian_withface(file);
    }

    //Close file
    file.close();
  }
  else if (format == "binary_big_endian"){
    //Open file
    std::ifstream file(path_file, ios::binary);

    //Read header
    this->Loader_header(file);

    //Read data
    if(face_number == 0){
      this->Loader_bin_big_endian(file);
    }else{
      this->Loader_bin_big_endian_withface(file);
    }

    //Close file
    file.close();
  }

  //---------------------------
  return data_out;
}

//Loader data
void file_PLY::Loader_header(std::ifstream& file){
  this->property_name.clear();
  this->property_type.clear();
  this->property_size.clear();
  this->property_number = 0;
  this->is_intensity = false;
  this->is_timestamp = false;
  this->is_normal = false;
  this->is_color = false;
  //---------------------------

  // Separate the header
  string line, h1, h2, h3, h4;
  bool vertex_ended = false;
  do{
    getline(file, line);
    std::istringstream iss(line);
    iss >> h1 >> h2 >> h3 >> h4;

    //Retrieve format
    if(h1 == "format") format = h2;

    //Retrieve number of point
    if(h1 + h2 == "elementvertex"){
      this->point_number = std::stoi(h3);
    }

    //Retrieve property
    if(h1 == "property" && vertex_ended == false){
      if (h2 == "float32" || h2 == "float"){
        property_type.push_back("float32");
        property_size.push_back(4);
      }
      else if (h2 == "float64" || h2 == "double"){
        property_type.push_back("float64");
        property_size.push_back(8);
      }
      else if (h2 == "uint8"){
        property_type.push_back("uint8");
        property_size.push_back(2);
      }
      else if (h2 == "uint16"){
        property_type.push_back("uint16");
        property_size.push_back(2);
      }
      else if (h2 == "int" || h2 == "int32"){
        property_type.push_back("uint32");
        property_size.push_back(4);
      }
      else if (h2 == "uchar"){
        property_type.push_back("uchar");
        property_size.push_back(1);
      }
      else{ // Default
        property_type.push_back("unknown");
        property_size.push_back(4);
      }

      if(h3 == "timestamp"){
        is_timestamp = true;
      }
      else if(h3 == "nx"){
        is_normal = true;
      }
      else if(h3 == "red"){
        is_color = true;
      }
      else if(h3 == "scalar_field" || h3 == "scalar_Scalar_field" || h3 == "intensity"){
        is_intensity = true;
      }

      property_name.push_back(h3);
      property_number++;
    }

    //Retrieve property
    if(h1 + h2 == "elementface"){
      vertex_ended = true;
      this->face_number = std::stoi(h3);
    }
  }while (line.find("end_header") != 0);

  //---------------------------
}
void file_PLY::Loader_ascii(std::ifstream& file){
  vector<vec3> vertex;
  vector<vec3> normal;
  vector<float> intensity;
  //---------------------------

  //Retrieve vertex data
  string line;
  int cpt = 0;
  while (std::getline(file, line)){
    //Check vertex number
    if(cpt == point_number){
      break;
    }
    cpt++;

    //Stocke all line values
    std::istringstream iss(line);
    vector<float> data;
    for(int i=0; i<property_number; i++){
      float d;
      iss >> d;
      data.push_back(d);
    }

    //Location
    int id_x = get_id_property("x");
    if(id_x != -1){
      vertex.push_back(vec3(data[id_x], data[id_x+1], data[id_x+2]));
    }

    //Normal
    int id_nx = get_id_property("nx");
    if(id_nx != -1){
      normal.push_back(vec3(data[id_nx], data[id_nx+1], data[id_nx+2]));
    }

    //Intensity
    int id_i = get_id_property("intensity");
    if(id_i != -1){
      intensity.push_back(data[id_i]);
    }
  }

  data_out->xyz = vertex;
  data_out->Nxyz = normal;
  data_out->I = intensity;

  //---------------------------
  data_out->size = data_out->xyz.size();
}
void file_PLY::Loader_ascii_withface(std::ifstream& file){
  vector<vec3> vertex;
  vector<vec3> normal;
  vector<float> intensity;
  //---------------------------

  //Retrieve vertex data
  string line;
  for(int i=0; i<point_number; i++){
    //Get line
    std::getline(file, line);
    std::istringstream iss(line);

    //Stocke all line values
    vector<float> data;
    float d;
    for(int i=0; i<property_number; i++){
      iss >> d;
      data.push_back(d);
    }

    //Location
    int id_x = get_id_property("x");
    if(id_x != -1){
      vertex.push_back(vec3(data[id_x], data[id_x+1], data[id_x+2]));
    }

    //Normal
    int id_nx = get_id_property("nx");
    if(id_nx != -1){
      normal.push_back(vec3(data[id_nx], data[id_nx+1], data[id_nx+2]));
    }

    //Intensity
    int id_i = get_id_property("intensity");
    if(id_i != -1){
      intensity.push_back(data[id_i]);
    }
  }

  //Retrieve face data
  while (std::getline(file, line)){
    std::istringstream iss(line);
    float nb_vertice;
    iss >> nb_vertice;

    //Stocke all line index
    vector<int> idx;
    float d;
    for(int i=0; i<nb_vertice; i++){
      iss >> d;
      idx.push_back(d);
    }

    //Retrieve face data
    for(int i=0; i<nb_vertice; i++){
      data_out->xyz.push_back(vertex[idx[i]]);
      if(get_id_property("nx") != -1){
        data_out->Nxyz.push_back(normal[idx[i]]);
      }
      if(get_id_property("intensity") != -1){
        data_out->I.push_back(intensity[idx[i]]);
      }
    }

    //Deduce drawing type
    if(nb_vertice == 3){
      data_out->draw_type_name = "triangle";
    }
    else if(nb_vertice == 4){
      data_out->draw_type_name = "quad";
    }
  }

  //---------------------------
  data_out->size = data_out->xyz.size();
}
void file_PLY::Loader_bin_little_endian(std::ifstream& file){
  //---------------------------

  //Read data
  int block_size = property_number * point_number * sizeof(double);
  char* block_data = new char[block_size];
  file.read(block_data, block_size);

  //Convert raw data into decimal data
  int offset = 0;
  vector<vector<float>> block_vec;
  block_vec.resize(property_number, vector<float>(point_number));
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      if(property_type[j] == "float32"){
        float value = get_float_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
      else if(property_type[j] == "float64"){
        float value = get_double_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
      else if(property_type[j] == "uint8"){
        float value = get_uint8_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
      else if(property_type[j] == "uint16"){
        float value = get_uint16_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
      else if(property_type[j] == "uint32"){
        float value = get_uint32_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
      else if(property_type[j] == "uchar"){
        float value = get_uchar_from_binary(block_data, offset);
        block_vec[j][i] = value;
      }
    }
  }

  //Resize vectors accordingly
  data_out->xyz.resize(point_number, vec3(0,0,0));
  if(is_timestamp) data_out->ts.resize(point_number, 0);
  if(is_intensity) data_out->I.resize(point_number, 0);
  if(is_normal) data_out->Nxyz.resize(point_number, vec3(0,0,0));
  if(is_color) data_out->rgb.resize(point_number, vec4(0,0,0,0));
  data_out->size = point_number;

  //Insert data in the adequate vector
  //#pragma omp parallel for
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(block_vec[j][i], block_vec[j+1][i], block_vec[j+2][i]);
        data_out->xyz[i] = point;
      }

      //Normal
      if(property_name[j] == "nx"){
        vec3 normal = vec3(block_vec[j][i], block_vec[j+1][i], block_vec[j+2][i]);
        data_out->Nxyz[i] = normal;
      }

      //Color
      if(property_name[j] == "red"){
        float red = block_vec[j][i] / 255;
        float green = block_vec[j+1][i] / 255;
        float blue = block_vec[j+2][i] / 255;
        vec4 rgb = vec4(red, green, blue, 1.0f);
        data_out->rgb[i] = rgb;
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = block_vec[j][i];
        data_out->I[i] = Is;
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = block_vec[j][i];
        data_out->ts[i] = ts;
      }
    }
  }

  //---------------------------
}
void file_PLY::Loader_bin_little_endian_withface(std::ifstream& file){
  //---------------------------

  //Read data
  int block_size = property_number * point_number * sizeof(float);
  char* block_data = new char[block_size];
  file.read(block_data, block_size);

  //Convert raw data into decimal data
  int offset = 0;
  vector<vector<float>> block_vec;
  block_vec.resize(property_number, vector<float>(point_number));
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      float value = get_float_from_binary(block_data, offset);
      block_vec[j][i] = value;
    }
  }

  //Insert data in the adequate vector
  vector<vec3> vertex;
  vector<vec3> normal;
  vector<float> intensity;
  vector<float> timestamp;
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(block_vec[j][i], block_vec[j+1][i], block_vec[j+2][i]);
        vertex.push_back(point);
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = block_vec[j][i];
        intensity.push_back(Is);
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = block_vec[j][i];
        timestamp.push_back(ts);
      }
    }
  }

  //Get face index
  int block_size_id = 4 * face_number * sizeof(int);
  char* block_data_id = new char[block_size_id];
  file.read(block_data_id, block_size_id);

  //Convert raw data into decimal data
  offset = 0;
  int nb_vertice;
  for(int i=0; i<face_number; i++){
    //Get number of face vertices
    int value =  (int)*((u_char *) (block_data_id + offset));
    offset += sizeof(u_char);
    nb_vertice = value;

    //Get face vertices index
    vector<int> idx;
    for (int j=0; j<value; j++){
      int value =  *((int *) (block_data_id + offset));
      offset += sizeof(int);
      idx.push_back(value);
    }

    //Location
    for(int j=0; j<idx.size(); j++){
      data_out->xyz.push_back(vertex[idx[j]]);
    }
  }

  //Deduce drawing type
  if(nb_vertice == 3){
    data_out->draw_type_name = "triangle";
  }
  else if(nb_vertice == 4){
    data_out->draw_type_name = "quad";
  }

  //---------------------------
  data_out->size = data_out->xyz.size();
}
void file_PLY::Loader_bin_big_endian(std::ifstream& file){
  //---------------------------

  //Read data
  int block_size = property_number * point_number * sizeof(float);
  char* block_data = new char[block_size];
  file.read(block_data, block_size);

  //Convert raw data into decimal data
  int offset = 0;
  vector<vector<float>> block_vec;
  block_vec.resize(property_number, vector<float>(point_number));
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      float value = get_float_from_binary(block_data, offset);
      block_vec[j][i] = reverse_float(value);
    }
  }

  //Resize vectors accordingly
  data_out->xyz.resize(point_number, vec3(0,0,0));
  if(is_timestamp) data_out->ts.resize(point_number, 0);
  if(is_intensity) data_out->I.resize(point_number, 0);
  data_out->size = point_number;

  //Insert data in the adequate vector
  #pragma omp parallel for
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(block_vec[j][i], block_vec[j+1][i], block_vec[j+2][i]);
        data_out->xyz[i] = point;
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = block_vec[j][i];
        data_out->I[i] = Is;
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = block_vec[j][i];
        data_out->ts[i] = ts;
      }
    }
  }

  //---------------------------
}
void file_PLY::Loader_bin_big_endian_withface(std::ifstream& file){
  //---------------------------

  //Read data
  int block_size = property_number * point_number * sizeof(float);
  char* block_data = new char[block_size];
  file.read(block_data, block_size);

  //Convert raw data into decimal data
  int offset = 0;
  vector<vector<float>> block_vec;
  block_vec.resize(property_number, vector<float>(point_number));
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      float value = get_float_from_binary(block_data, offset);
      block_vec[j][i] = reverse_float(value);
    }
  }

  //Insert data in the adequate vector
  vector<vec3> vertex;
  vector<vec3> normal;
  vector<float> intensity;
  vector<float> timestamp;
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(block_vec[j][i], block_vec[j+1][i], block_vec[j+2][i]);
        vertex.push_back(point);
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = block_vec[j][i];
        intensity.push_back(Is);
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = block_vec[j][i];
        timestamp.push_back(ts);
      }
    }
  }

  //Get face index
  int block_size_id = 4 * face_number * sizeof(int);
  char* block_data_id = new char[block_size_id];
  file.read(block_data_id, block_size_id);

  //Convert raw data into decimal data
  offset = 0;
  int nb_vertice;
  for(int i=0; i<face_number; i++){
    //Get number of face vertices
    int value =  (int)*((u_char *) (block_data_id + offset));
    offset += sizeof(u_char);
    nb_vertice = value;

    //Get face vertices index
    vector<int> idx;
    for (int j=0; j<value; j++){
      int value =  *((int *) (block_data_id + offset));
      offset += sizeof(int);
      idx.push_back(reverse_int(value));
    }

    //Location
    for(int j=0; j<idx.size(); j++){
      data_out->xyz.push_back(vertex[idx[j]]);
    }
  }

  //Deduce drawing type
  if(nb_vertice == 3){
    data_out->draw_type_name = "triangle";
  }
  else if(nb_vertice == 4){
    data_out->draw_type_name = "quad";
  }

  //---------------------------
  data_out->size = data_out->xyz.size();
}

//Loader subfunctions
float file_PLY::reverse_float(const float inFloat){
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}
int file_PLY::reverse_int(const int inInt){
   int retVal;
   char *intToConvert = ( char* ) & inInt;
   char *returnInt = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnInt[0] = intToConvert[3];
   returnInt[1] = intToConvert[2];
   returnInt[2] = intToConvert[1];
   returnInt[3] = intToConvert[0];

   return retVal;
}
void file_PLY::reorder_by_timestamp(){
  vector<vec3> pos;
  vector<float> ts;
  vector<float> Is;
  //---------------------------

  if(data_out->ts.size() != 0){
    //Check for non void and reorder by index
    for (auto i: fct_sortByIndexes(data_out->ts)){
      if(data_out->xyz[i] != vec3(0, 0, 0)){
        //Location adn timestamp
        ts.push_back(data_out->ts[i]);
        pos.push_back(data_out->xyz[i]);

        //Intensity
        if(data_out->I.size() != 0){
          Is.push_back(data_out->I[i]);
        }
      }
    }

    //Set new vectors
    data_out->xyz = pos;
    data_out->ts = ts;
    data_out->I = Is;
  }

  //---------------------------
}
int file_PLY::get_id_property(string name){
  //---------------------------

  for(int i=0; i<property_name.size(); i++){
    if(property_name[i] == name){
      return i;
    }
  }

  //---------------------------
  return -1;
}
float file_PLY::get_float_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  *((float *) (block_data + offset));
  offset += sizeof(float);

  //---------------------------
  return value;
}
float file_PLY::get_double_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((double *) (block_data + offset));
  offset += sizeof(double);

  //---------------------------
  return value;
}
float file_PLY::get_int_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((int *) (block_data + offset));
  offset += sizeof(int);

  //---------------------------
  return value;
}
float file_PLY::get_uint8_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((uint8 *) (block_data + offset));
  offset += sizeof(uint8);

  //---------------------------
  return value;
}
float file_PLY::get_uint16_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((uint16 *) (block_data + offset));
  offset += sizeof(uint16);

  //---------------------------
  return value;
}
float file_PLY::get_uint32_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((uint32 *) (block_data + offset));
  offset += sizeof(uint32);

  //---------------------------
  return value;
}
float file_PLY::get_uchar_from_binary(char* block_data, int& offset){
  //---------------------------

  float value =  (float)*((u_char *) (block_data + offset));
  offset += sizeof(u_char);

  //---------------------------
  return value;
}

//Main exporter functions
bool file_PLY::exporter_collection(Collection* collection, string path_file, string ply_format){
  //---------------------------

  //Check for file format ending
  if(path_file.substr(path_file.find_last_of(".") + 1) != "ply"){
    path_file.append(".ply");
  }

  if (ply_format == "ascii"){
    for(int i=0; i<collection->nb_obj; i++){
      Object_* object = collection->get_obj(i);

      //Open file
      std::ofstream file(path_file);

      //Save header
      this->Exporter_header(file, ply_format, object, object->xyz.size());

      //Save data
      this->Exporter_data_ascii(file, object);

      file.close();
    }
  }
  else if (ply_format == "binary" || ply_format == "binary_little_endian"){
    for(int i=0; i<collection->nb_obj; i++){
      Object_* object = collection->get_obj(i);

      //Locak file
      int fd = open(path_file.c_str(), O_RDWR | O_CREAT, 0666);
      flock(fd, LOCK_EX | LOCK_NB);

      //Open file
      std::ofstream file(path_file, ios::binary);

      //Save header
      this->Exporter_header(file, "binary_little_endian", object, object->xyz.size());

      //Save data
      this->Exporter_data_binary(file, object);

      file.close();
    }
  }
  else{
    cout << "WARNING: format not recognized " << ply_format << endl;
    return false;
  }

  //---------------------------
  return true;
}
bool file_PLY::Exporter_subset(string path_dir, string ply_format, Cloud* cloud){
  string filePath = path_dir + cloud->name + ".tmp";
  string filePath_end = path_dir + cloud->name + ".ply";
  //---------------------------

  //Check for file format ending
  if (ply_format == "ascii"){

    //Open file
    std::ofstream file(filePath);

    //Save header
    this->Exporter_header(file, ply_format, cloud, cloud->xyz.size());

    //Save data
    this->Exporter_data_ascii(file, cloud);

    file.close();
  }
  else if (ply_format == "binary" || ply_format == "binary_little_endian"){
    ply_format = "binary_little_endian";

    //Open file
    std::ofstream file(filePath, ios::binary);

    //Save header
    this->Exporter_header(file, ply_format, cloud, cloud->xyz.size());

    //Save data
    this->Exporter_data_binary(file, cloud);

    file.close();

  }
  else{
    cout << "WARNING: format not recognized " << ply_format << endl;
    return false;
  }

  //Rename file in proper format when complete
  rename(filePath.c_str(), filePath_end.c_str());

  //---------------------------
  return true;
}
bool file_PLY::Exporter_subset(string path_dir, string ply_format, Cloud* cloud, string fileName){
  string filePath = path_dir + fileName + ".ply";
  //---------------------------

  //Check for file format ending
  if (ply_format == "ascii"){

    //Open file
    std::ofstream file(filePath);

    //Save header
    this->Exporter_header(file, ply_format, cloud, cloud->xyz.size());

    //Save data
    this->Exporter_data_ascii(file, cloud);

    file.close();
  }
  else if (ply_format == "binary" || ply_format == "binary_little_endian"){
    ply_format = "binary_little_endian";

    //Locak file
    int fd = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    flock(fd, LOCK_EX | LOCK_NB);

    //Open file
    std::ofstream file(filePath, ios::binary);

    //Save header
    this->Exporter_header(file, ply_format, cloud, cloud->xyz.size());

    //Save data
    this->Exporter_data_binary(file, cloud);

    file.close();

  }
  else{
    cout << "WARNING: format not recognized " << ply_format << endl;
    return false;
  }

  //---------------------------
  return true;
}
bool file_PLY::Exporter_set(string path_dir, string ply_format, Collection* collection, int ID, int nb){
  Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), ID);
  string filePath = path_dir + cloud->name + ".tmp";
  string filePath_end = path_dir + cloud->name + ".ply";
  //---------------------------

  //Check for file format ending
  if (ply_format == "binary" || ply_format == "binary_little_endian"){
    ply_format = "binary_little_endian";

    //Open file
    std::ofstream file(filePath, ios::binary);

    //Get total number of point
    int nb_point = 0;
    for(int i=0; i<nb; i++){
      int ID_object = ID - i;
      if(ID_object >= 0){
        Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), ID - i);
        nb_point += cloud->xyz.size();
      }
    }

    //Save header
    this->Exporter_header(file, ply_format, cloud, nb_point);

    //Save data
    for(int i=0; i<nb; i++){
      int ID_object = ID - i;
      if(ID_object >= 0){
        Cloud* cloud = (Cloud*)*next(collection->list_obj.begin(), ID - i);
        this->Exporter_data_binary(file, cloud);
      }
    }

    file.close();

  }
  else{
    cout << "WARNING: format not compatible for set exporting" << endl;
    return false;
  }

  //Rename file in proper format when complete
  rename(filePath.c_str(), filePath_end.c_str());

  //---------------------------
  return true;
}

//Exporter subfunctions
void file_PLY::Exporter_header(std::ofstream& file, string format, Object_* object, int nb_point){
  this->point_number = nb_point;
  this->property_number = 3;
  this->property_name.clear();
  //---------------------------

  //Write header
  file << "ply" << endl;
  file << "format " + format + " 1.0" << endl;
  file << "element vertex " << point_number << endl;

  file << "property float32 x" << endl;
  file << "property float32 y" << endl;
  file << "property float32 z" << endl;
  property_name.push_back("x");
  property_name.push_back("y");
  property_name.push_back("z");

  if(object->has_color){
    file << "property uchar red" << endl;
    file << "property uchar green" << endl;
    file << "property uchar blue" << endl;

    property_name.push_back("r");
    property_name.push_back("g");
    property_name.push_back("b");
    property_number += 3;
  }
  if(object->has_normal != 0){
    file << "property float32 nx" << endl;
    file << "property float32 ny" << endl;
    file << "property float32 nz" << endl;

    property_name.push_back("nx");
    property_name.push_back("ny");
    property_name.push_back("nz");
    property_number += 3;
  }
  if(object->has_intensity != 0){
    file << "property float32 scalar_field" << endl;

    property_name.push_back("I");
    property_number++;
  }
  if(object->has_timestamp != 0){
    file << "property float32 timestamp" << endl;

    property_name.push_back("ts");
    property_number++;
  }
  file << "end_header" <<endl;

  //---------------------------
}
void file_PLY::Exporter_data_ascii(std::ofstream& file, Object_* object){
  vector<vec3>& XYZ = object->xyz;
  vector<vec4>& RGB = object->rgb;
  vector<vec3>& N = object->Nxyz;
  int precision = 6;
  //---------------------------

  //If cloud get intensity vector
  vector<float> Is;
  if(object->obj_type == "cloud"){
    Cloud* cloud = (Cloud*)object;
    Is = cloud->I;
  }

  //Write data in the file
  for(int i=0; i<XYZ.size(); i++){
    file << fixed;

    //Location
    file << setprecision(precision) << XYZ[i].x <<" "<< XYZ[i].y <<" "<< XYZ[i].z <<" ";

    //Color
    if(object->has_color){
      file << setprecision(0) << RGB[i].x * 255 <<" "<< RGB[i].y * 255 <<" "<< RGB[i].z * 255 <<" ";
    }

    //Normal
    if(object->Nxyz.size() != 0){
      file << setprecision(precision) << N[i].x <<" "<< N[i].y <<" "<< N[i].z <<" ";
    }

    //Intensity
    if(Is.size() != 0){
      float Is_scaled = (Is[i]*4096)-2048;
      file << setprecision(0) << Is_scaled << " ";
    }

    file << endl;
  }

  //---------------------------
}
void file_PLY::Exporter_data_binary(std::ofstream& file, Object_* object){
  //---------------------------

  //If cloud get intensity vector
  vector<float> Is;
  if(object->obj_type == "cloud"){
    Cloud* cloud = (Cloud*)object;
    Is = cloud->I;
  }

  //Prepare data writing by blocks
  int block_size = property_number * object->xyz.size() * sizeof(float);
  char* block_data = new char[block_size];

  //Convert decimal data into binary data
  int offset = 0;
  int cpt_property = 0;
  for (int i=0; i<object->xyz.size(); i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        for(int k=0; k<3; k++){
          memcpy(block_data + offset, &object->xyz[i][k], sizeof(float));
          offset += sizeof(float);
        }
      }

      //Color
      if(property_name[j] == "r"){
        for(int k=0; k<3; k++){
          int color_int = object->rgb[i][k] * 255;
          memcpy(block_data + offset, &color_int, sizeof(u_char));
          offset += sizeof(u_char);
        }
      }

      //Intensity
      if(property_name[j] == "I"){
        memcpy(block_data + offset, &Is[i], sizeof(float));
        offset += sizeof(float);
      }

      //Normal
      if(property_name[j] == "nx"){
        for(int k=0; k<3; k++){
          memcpy(block_data + offset, &object->Nxyz[i][k], sizeof(float));
          offset += sizeof(float);
        }
      }

      //Timestamp
      if(property_name[j] == "ts"){
        memcpy(block_data + offset, &object->ts[i], sizeof(float));
        offset += sizeof(float);
      }
    }
  }

  //Read all data blocks & read the last data block
  file.write(block_data, block_size);

  //---------------------------
}
