#include "file_PLY.h"

#include "../../Specific/fct_maths.h"

#include <iomanip>
#include <sys/file.h>


//Constructor / Destructor
file_PLY::file_PLY(){}
file_PLY::~file_PLY(){}

//Main loader functions
dataFile* file_PLY::Loader(string path_file){
  data_out = new dataFile();
  string nameFormat = path_file.substr(path_file.find_last_of("/\\") + 1);
  data_out->name = nameFormat.substr(0, nameFormat.find_last_of("."));
  data_out->path = path_file;
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
    this->Loader_data_ascii(file);

    file.close();

  }
  else if (format == "binary_little_endian"){
    //Open file
    std::ifstream file(path_file, ios::binary);

    //Read header
    this->Loader_header(file);

    //Read data
    this->Loader_data_binary(file);

    //Close file
    file.close();
  }
  else if (format == "binary_big_endian"){
    cout << "WARNING: function not implemented for binary big endian file" << endl;
  }

  //---------------------------
  return data_out;
}

//Loader subfunctions
void file_PLY::Loader_header(std::ifstream& file){
  this->property_name.clear();
  this->property_type.clear();
  this->property_size.clear();
  this->property_number = 0;
  this->is_intensity = false;
  this->is_timestamp = false;
  //---------------------------

  // Separate the header
  string line, h1, h2, h3, h4;
  do{
    getline(file, line);
    std::istringstream iss(line);
    iss >> h1 >> h2 >> h3 >> h4;

    //Retrieve format
    if(h1 == "format") format = h2;

    //Retrieve number of point
    if(h1 + h2 == "elementvertex"){
      point_number = std::stoi(h3);
    }

    //Retrieve property
    if(h1 == "property"){
      if (h2 == "float32" | h2 == "float"){
        property_type.push_back("float32");
        property_size.push_back(4);
      }
      else if (h2 == "float64" | h2 == "double"){
        property_type.push_back("float64");
        property_size.push_back(8);
      }
      else if (h2 == "int"){
        property_type.push_back("int32");
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
      if(h3 == "scalar_Scalar_field" || h3 == "intensity"){
        is_intensity = true;
      }

      property_name.push_back(h3);
      property_number++;
    }
  }while (line.find("end_header") != 0);

  //---------------------------
}
void file_PLY::Loader_data_ascii(std::ifstream& file){
  //---------------------------

  //Retrieve data
  string line;
  int cpt = 1;
  while (std::getline(file, line)){
    std::istringstream iss(line);

    //Stocke all line values
    float d0, d1, d2, d3, d4, d5, d6, d7, d8, d9, d10;
    iss >> d0 >> d1 >> d2 >> d3 >> d4 >> d5 >> d6 >> d7 >> d8 >> d9 >> d10;

    //Location
    data_out->location.push_back(vec3(d0, d1, d2));

    //Normal
    data_out->normal.push_back(vec3(d3, d4, d5));

    //Check for data end
    if(cpt >= point_number){
      break;
    }
    cpt++;
  }

  //---------------------------
  data_out->size = data_out->location.size();
}
void file_PLY::Loader_data_binary(std::ifstream& file){
  //---------------------------

  //Read data
  int buffer_size = property_number * point_number * sizeof(float);
  char* data = new char[buffer_size];
  file.read(data, buffer_size);

  //Convert raw data into decimal data
  int offset = 0;
  vector<vector<float>> data_columns;
  data_columns.resize(property_number, vector<float>(point_number));
  for (int i=0; i<point_number; i++){
    //Get data for each property
    for (int j=0; j<property_number; j++){
      float value =  *((float *) (data + offset));
      offset += sizeof(float);
      data_columns[j][i] = value;
    }
  }

  //Insert data in the adequate vector
  data_out->location.resize(point_number, vec3(0,0,0));
  if(is_timestamp) data_out->timestamp.resize(point_number, 0);
  if(is_intensity) data_out->intensity.resize(point_number, 0);
  data_out->size = point_number;

  #pragma omp parallel for
  for (int i=0; i<point_number; i++){
    for (int j=0; j<property_number; j++){
      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(data_columns[j][i], data_columns[j+1][i], data_columns[j+2][i]);
        data_out->location[i] = point;
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = data_columns[j][i];
        data_out->intensity[i] = Is;
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = data_columns[j][i];
        data_out->timestamp[i] = ts;
      }
    }
  }

  //---------------------------
}
void file_PLY::reorder_by_timestamp(){
  vector<vec3> pos;
  vector<float> ts;
  vector<float> Is;
  //---------------------------

  if(data_out->timestamp.size() != 0){
    //Check for non void and reorder by index
    for (auto i: fct_sortByIndexes(data_out->timestamp)){
      if(data_out->location[i] != vec3(0, 0, 0)){
        //Location adn timestamp
        ts.push_back(data_out->timestamp[i]);
        pos.push_back(data_out->location[i]);

        //Intensity
        if(data_out->intensity.size() != 0){
          Is.push_back(data_out->intensity[i]);
        }
      }
    }

    //Set new vectors
    data_out->location = pos;
    data_out->timestamp = ts;
    data_out->intensity = Is;
  }

  //---------------------------
}

//Main exporter functions
bool file_PLY::Exporter_cloud(string path_file, string ply_format, Cloud* cloud){
  //---------------------------

  //Check for file format ending
  if(path_file.substr(path_file.find_last_of(".") + 1) != "ply"){
    path_file.append(".ply");
  }

  if (ply_format == "ascii"){
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = *next(cloud->subset.begin(), i);

      //Open file
      std::ofstream file(path_file);

      //Save header
      this->Exporter_header(file, ply_format, subset);

      //Save data
      this->Exporter_data_ascii(file, subset);

      file.close();
    }
  }
  else if (format == "binary" || format == "binary_little_endian"){
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = *next(cloud->subset.begin(), i);
      format = "binary_little_endian";

      //Locak file
      int fd = open(path_file.c_str(), O_RDWR | O_CREAT, 0666);
      flock(fd, LOCK_EX | LOCK_NB);

      //Open file
      std::ofstream file(path_file, ios::binary);

      //Save header
      this->Exporter_header(file, ply_format, subset);

      //Save data
      this->Exporter_data_binary(file, subset);

      file.close();
    }
  }
  else{
    cout << "WARNING: format not recognized" << endl;
    return false;
  }

  //---------------------------
  return true;
}
bool file_PLY::Exporter_subset(string path_dir, string ply_format, Subset* subset){
  string filePath = path_dir + subset->name + ".tmp";
  string filePath_end = path_dir + subset->name + ".ply";
  //---------------------------

  //Check for file format ending
  if (ply_format == "ascii"){

    //Open file
    std::ofstream file(filePath);

    //Save header
    this->Exporter_header(file, ply_format, subset);

    //Save data
    this->Exporter_data_ascii(file, subset);

    file.close();
  }
  else if (ply_format == "binary" || ply_format == "binary_little_endian"){
    ply_format = "binary_little_endian";

    //Locak file
    //int fd = open(filePath.c_str(), O_RDWR | O_CREAT, 0666);
    //flock(fd, LOCK_EX | LOCK_NB);

    //Open file
    std::ofstream file(filePath, ios::binary);

    //Save header
    this->Exporter_header(file, ply_format, subset);

    //Save data
    this->Exporter_data_binary(file, subset);

    file.close();

  }
  else{
    cout << "WARNING: format not recognized" << endl;
    return false;
  }

  //Rename file in proper format when complete
  rename(filePath.c_str(), filePath_end.c_str());

  //---------------------------
  return true;
}
bool file_PLY::Exporter_subset(string path_dir, string ply_format, Subset* subset, string fileName){
  string filePath = path_dir + fileName + ".ply";
  //---------------------------

  //Check for file format ending
  if (ply_format == "ascii"){

    //Open file
    std::ofstream file(filePath);

    //Save header
    this->Exporter_header(file, ply_format, subset);

    //Save data
    this->Exporter_data_ascii(file, subset);

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
    this->Exporter_header(file, ply_format, subset);

    //Save data
    this->Exporter_data_binary(file, subset);

    file.close();

  }
  else{
    cout << "WARNING: format not recognized" << endl;
    return false;
  }

  //---------------------------
  return true;
}

//Exporter subfunctions
void file_PLY::Exporter_header(std::ofstream& file, string format, Subset* subset){
  this->point_number = subset->xyz.size();
  this->property_number = 3;
  this->property_name.clear();
  //---------------------------

  //Write header
  file << "ply" << endl;
  file << "ID " << subset->ID << endl;
  file << "format " + format + " 1.0" << endl;
  file << "element vertex " << point_number << endl;
  file << "property float x" << endl;
  file << "property float y" << endl;
  file << "property float z" << endl;
  if(subset->has_color){
    file << "property uchar red" << endl;
    file << "property uchar green" << endl;
    file << "property uchar blue" << endl;

    property_number += 3;
  }
  if(subset->N.size() != 0){
    file << "property float nx" << endl;
    file << "property float ny" << endl;
    file << "property float nz" << endl;

    property_number += 3;
  }
  if(subset->I.size() != 0){
    file << "property float scalar_Scalar_field" << endl;

    property_number++;
  }
  if(subset->ts.size() != 0){
    file << "property float timestamp" << endl;

    property_number++;
  }
  file << "end_header" <<endl;

  //---------------------------
}
void file_PLY::Exporter_data_ascii(std::ofstream& file, Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vector<vec4>& RGB = subset->RGB;
  vector<vec3>& N = subset->N;
  vector<float>& Is = subset->I;
  int precision = 6;
  //---------------------------

  //Write data in the file
  for(int i=0; i<XYZ.size(); i++){
    file << fixed;

    //Location
    file << setprecision(precision) << XYZ[i].x <<" "<< XYZ[i].y <<" "<< XYZ[i].z <<" ";

    //Color
    if(subset->has_color){
      file << setprecision(0) << RGB[i].x * 255 <<" "<< RGB[i].y * 255 <<" "<< RGB[i].z * 255 <<" ";
    }

    //Normal
    if(subset->N.size() != 0){
      file << setprecision(precision) << N[i].x <<" "<< N[i].y <<" "<< N[i].z <<" ";
    }

    //Intensity
    if(subset->I.size() != 0){
      float Is_scaled = (Is[i]*4096)-2048;
      file << setprecision(0) << Is_scaled << " ";
    }

    file << endl;
  }

  //---------------------------
}
void file_PLY::Exporter_data_binary(std::ofstream& file, Subset* subset){
  //---------------------------

  //Prepare data writing by blocks
  int buffer_size = property_number * point_number * sizeof(float);
  char* data = new char[buffer_size];

  //Convert decimal data into binary data
  int offset = 0;
  for (int i=0; i<point_number; i++){
    //Location
    for(int j=0; j<3; j++){
      memcpy(data + offset, &subset->xyz[i][j], sizeof(float));
      offset += sizeof(float);
    }

    //Intensity
    if(subset->I.size() != 0){
      memcpy(data + offset, &subset->I[i], sizeof(float));
      offset += sizeof(float);
    }

    //Timestamp
    if(subset->ts.size() != 0){
      memcpy(data + offset, &subset->ts[i], sizeof(float));
      offset += sizeof(float);
    }
  }

  //Read all data blocks & read the last data block
  file.write(data, buffer_size);

  //---------------------------
}
