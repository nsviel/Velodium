#include "file_PLY.h"

#include "../../Specific/fct_maths.h"

#include <iomanip>


//Constructor / Destructor
filePLY::filePLY(){}
filePLY::~filePLY(){}

//Main functions
dataFile* filePLY::Loader(string pathFile){
  data_out = new dataFile();
  data_out->name = "";
  data_out->path = pathFile;
  //---------------------------

  //Get format type
  std::ifstream file(pathFile);
  this->Loader_header(file);

  //Open data
  if (format == "ascii"){

    //Open file
    std::ifstream file(pathFile);

    //Read header
    this->Loader_header(file);

    //Read data
    this->Loader_data_ascii(file);

    file.close();

  }
  else if (format == "binary_little_endian"){

    //Open file
    std::ifstream file(pathFile, ios::binary);

    //Read header
    this->Loader_header(file);

    //Read data
    this->Loader_data_binary_auto(file);

    file.close();

  }
  else if (format == "binary_big_endian"){
    cout << "WARNING: function not implemented for binary big endian file" << endl;
  }

  //---------------------------
  return data_out;
}
bool filePLY::Exporter_cloud(string pathFile, string ply_format, Cloud* cloud){
  //---------------------------

  //Check for file format ending
  if(pathFile.substr(pathFile.find_last_of(".") + 1) != "ply"){
    pathFile.append(".ply");
  }

  if (ply_format == "ascii"){
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = &cloud->subset[i];

      //Open file
      std::ofstream file(pathFile);

      //Save header
      this->Exporter_header(file, ply_format, subset);

      //Save data
      this->Exporter_data_ascii(file, subset);

      file.close();
    }
  }
  else if (format == "binary" || format == "binary_little_endian"){
    for(int i=0; i<cloud->nb_subset; i++){
      Subset* subset = &cloud->subset[i];
      format = "binary_little_endian";

      //Open file
      std::ofstream file(pathFile, ios::binary);

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
bool filePLY::Exporter_subset(string dirPath, string ply_format, Subset* subset){
  string filePath = dirPath + subset->name + ".ply";
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
bool filePLY::Exporter_subset(string dirPath, string ply_format, Subset* subset, string fileName){
  string filePath = dirPath + fileName + ".ply";
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

//Loader subfunctions
void filePLY::Loader_header(std::ifstream& file){
  //---------------------------

  // Separate the header
  string header_str, header_temp;
  header_size = 0;
  do{
    getline(file, header_temp);
    header_str += header_temp + "\n";
    header_size++;
  }while (header_temp.find("end_header") != 0);
  stringstream header(header_str);

  // Parse header
  string line;
  property_type.clear();
  property_size.clear();
  property_number = 0;
  while (std::getline(header, line)){
    string h1, h2, h3, h4;
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

      property_name.push_back(h3);
      property_number++;
    }
  }

  //---------------------------
}
void filePLY::Loader_data_ascii(std::ifstream& file){
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
void filePLY::Loader_data_binary(std::ifstream& file){
  int DATA_BLOCK_SIZE = 16000;
  //---------------------------

  // Separate the header
  string header;
  do{
    getline(file, header);
  }while (header.find("end_header") != 0);

  //Prepare data reading by blocks
  int buffer = property_number * point_number * sizeof(float);
	char* data = new char[buffer];
	int data_block = buffer / DATA_BLOCK_SIZE;
	int data_block_rest = buffer % DATA_BLOCK_SIZE;

  //Read all data blocks & read the last data block
  for(int i=0; i<data_block; i++){
    file.read(data + i * DATA_BLOCK_SIZE, DATA_BLOCK_SIZE);
  }
	file.read(data + data_block * DATA_BLOCK_SIZE, data_block_rest);

  //Convert raw data into decimal data
  for (int i=0; i<point_number; i++){

      int offset = i * property_number * sizeof(float);

      vec3 point;
      point.x = *((float *) (data + offset));
      offset += sizeof(float);
      point.y = *((float *) (data + offset));
      offset += sizeof(float);
      point.z = *((float *) (data + offset));
      offset += sizeof(float);
      data_out->location.push_back(point);

      float timestamp;
      timestamp = *((float*)(data + offset));
      offset += sizeof(float);
      data_out->timestamp.push_back(timestamp);
  }

  //---------------------------
  data_out->size = data_out->location.size();
}
void filePLY::Loader_data_binary_auto(std::ifstream& file){
  //Adapt automatically to the property field name
  int DATA_BLOCK_SIZE = 16000;
  //---------------------------

  //Prepare data reading by blocks
  int buffer = property_number * point_number * sizeof(float);
	char* data = new char[buffer];
	int data_block = buffer / DATA_BLOCK_SIZE;
	int data_block_rest = buffer % DATA_BLOCK_SIZE;

  //Read all data blocks & read the last data block
  for(int i=0; i<data_block; i++){
    file.read(data + i * DATA_BLOCK_SIZE, DATA_BLOCK_SIZE);
  }
	file.read(data + data_block * DATA_BLOCK_SIZE, data_block_rest);

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
  for (int i=0; i<point_number; i++){

    for (int j=0; j<property_number; j++){

      //Location
      if(property_name[j] == "x"){
        vec3 point = vec3(data_columns[j][i], data_columns[j+1][i], data_columns[j+2][i]);
        data_out->location.push_back(point);
      }

      //Intensity
      if(property_name[j] == "scalar_Scalar_field" || property_name[j] == "intensity"){
        float Is = data_columns[j][i];
        data_out->intensity.push_back(Is);
      }

      //Timestamp
      if(property_name[j] == "timestamp"){
        float ts = data_columns[j][i];
        data_out->timestamp.push_back(ts);
      }
    }
  }

  //Reorder points in function of their timestamp
  vector<vec3> pos;
  vector<float> ts;
  if(data_out->timestamp.size() != 0){
    for (auto i: fct_sortByIndexes(data_out->timestamp)){
      ts.push_back(data_out->timestamp[i]);
      pos.push_back(data_out->location[i]);
      pos.push_back(data_out->location[i]);
    }

    data_out->location = pos;
    data_out->timestamp = ts;
  }

  data_out->size = data_out->location.size();

  //---------------------------
}

//Exporter subfunctions
void filePLY::Exporter_header(std::ofstream& file, string format, Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  point_number = XYZ.size();
  property_number = 4;
  //---------------------------

  //Write header
  file << "ply" <<endl;
  file << "format "+ format +" 1.0" <<endl;
  file << "element vertex " << point_number << endl;
  file << "property float x" << endl;
  file << "property float y" << endl;
  file << "property float z" << endl;
  if(subset->has_color){
    file << "property uchar red" << endl;
    file << "property uchar green" << endl;
    file << "property uchar blue" << endl;

    property_number++;
  }
  if(subset->N.size() != 0){
    file << "property float nx" << endl;
    file << "property float ny" << endl;
    file << "property float nz" << endl;

    property_number++;
  }
  if(subset->I.size() != 0){
    file << "property float scalar_Scalar_field" << endl;

    property_number++;
  }
  if(subset->ts.size() != 0){
    file << "property float timestamp" << endl;

    property_number++;
  }
  file << "element face 0" << endl;
  file << "property list uchar int vertex_indices" << endl;
  file << "end_header" <<endl;

  //---------------------------
}
void filePLY::Exporter_data_ascii(std::ofstream& file, Subset* subset){
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
void filePLY::Exporter_data_binary(std::ofstream& file, Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vector<vec4>& RGB = subset->RGB;
  vector<vec3>& N = subset->N;
  vector<float>& Is = subset->I;
  vector<float>& ts = subset->ts;
  int DATA_BLOCK_SIZE = 16000;
  //---------------------------

  //Prepare data writing by blocks
  int buffer = property_number * point_number * sizeof(float);
	char* data = new char[buffer];
	int data_block = buffer / DATA_BLOCK_SIZE;
	int data_block_rest = buffer % DATA_BLOCK_SIZE;

  //Convert decimal data into binary data
  int offset = 0;
  for (int i=0; i<point_number; i++){

    //Location
    for (int j=0; j<3; j++){
      memcpy(data + offset, &XYZ[i][j], sizeof(float));
      offset += sizeof(float);
    }

    //Intensity
    if(subset->I.size() != 0){
      memcpy(data + offset, &Is[i], sizeof(float));
      offset += sizeof(float);
    }

    //Timestamp
    if(subset->ts.size() != 0){
      memcpy(data + offset, &ts[i], sizeof(float));
      offset += sizeof(float);
    }

    offset += sizeof(float); // I don't know why a second offset is necessary (??)
  }

  //Read all data blocks & read the last data block
  for(int i=0; i<data_block; i++){
    file.write(data + i * DATA_BLOCK_SIZE, DATA_BLOCK_SIZE);
  }
	file.write(data + data_block * DATA_BLOCK_SIZE, data_block_rest);

  //---------------------------
}