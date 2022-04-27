#include "file_OBJ.h"

#include <fstream>


//Constructor / Destructor
file_OBJ::file_OBJ(){}
file_OBJ::~file_OBJ(){}

//Main function
dataFile* file_OBJ::Loader(string filePath){
  std::ifstream infile(filePath);
  std::string line;
  float a, b, c;
  string ID;
  dataFile* data = new dataFile();
  //---------------------------

  float R = float(rand()%101)/100;
  float G = float(rand()%101)/100;
  float B = float(rand()%101)/100;

  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> ID >> a >> b >> c;

    //Data extraction
    if(ID == "v" ){
      data->location.push_back(vec3(a, b, c));
    }
    if(ID == "vn"){
      data->normal.push_back(vec3(a, b, c));
    }
  }

  //---------------------------
  return data;
}

dataFile* file_OBJ::Loader_complete(string filePath){
  dataFile* data = new dataFile();
  //---------------------------

  //Prepare data
  std::ifstream infile(filePath);
  vector<vec4> location_idx;
  vector<vec4> normal_idx;
  vector<vec3> location;
  vector<vec3> normal;
  std::string line;
  float a, b, c;
  string ID;

  //Check file opening
  FILE* file = fopen(filePath.c_str(), "r");
  if(file == NULL){
    cout<<"Failed to open the file"<<endl;
  }

  //Reading loop
  while(1){
    char lineHeader[128];

    // read the first word of the line
    int res = fscanf(file, "%s", lineHeader);
    if(res == EOF){
      break;
    }

    //Location
    if(strcmp(lineHeader, "v") == 0){
      glm::vec3 vertex;
      int err = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
      location.push_back(vertex);
    }
    //Texture
    else if(strcmp(lineHeader, "vt") == 0){
      glm::vec2 uv;
      int err = fscanf(file, "%f %f\n", &uv.x, &uv.y );
    }
    //Normal
    else if(strcmp(lineHeader, "vn") == 0){
      glm::vec3 nxy;
      int err = fscanf(file, "%f %f %f\n", &nxy.x, &nxy.y, &nxy.z );
      normal.push_back(nxy);
    }
    //Face
    else if(strcmp(lineHeader, "f") == 0){
      vec4 vertex_id, uv_id, normal_id;
      int matches = fscanf(file, "%f/%f/%f %f/%f/%f %f/%f/%f %f/%f/%f\n", &vertex_id.x, &uv_id.x, &normal_id.x,
        &vertex_id.y, &uv_id.y, &normal_id.y,
        &vertex_id.z, &uv_id.z, &normal_id.z,
        &vertex_id.w, &uv_id.w, &normal_id.w);
      location_idx.push_back(vertex_id);
      normal_idx.push_back(normal_id);
    }
  }

  //Indexation
  for(int i=0; i<location_idx.size(); i++){
    for(int j=0; j<4; j++){
      data->location.push_back(location[location_idx[i][j] - 1]);
      data->normal.push_back(normal[normal_idx[i][j] - 1]);
    }
  }

  //---------------------------
  return data;
}
