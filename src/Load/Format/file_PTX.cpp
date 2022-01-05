#include "file_PTX.h"

//Constructor / Destructor
file_PTX::file_PTX(){
  //---------------------------

  this->option_separateCloud = false;
  this->option_scannerAtOrigin = false;
  this->option_applyCloudTransfo = true;
  this->retrieve_I = true;
  this->retrieve_RGB = true;
  this->retrieve_N = true;
  this->IdataFormat = 0;

  //---------------------------
}
file_PTX::~file_PTX(){}

//Main functions
dataFile* file_PTX::Loader(string pathFile){
  list_ptxCloud = new list<PTXCloud*>;
  PTXCloud* cloud = new PTXCloud;
  data_out = new dataFile();
  data_out->path = pathFile;
  //---------------------------

  //Open file
  std::ifstream infile(pathFile);

  //Data loop
  PC_line = 0;
  std::string line;
  while (std::getline(infile, line)){
    std::istringstream iss(line);
    x = 0; y = 0; z = 0; I = 0; r = 0; g = 0; b = 0;
    iss >> x >> y >> z >> I >> r >> g >> b;

    //New header detected -> new cloud
    if(abs(y) == 0 && abs(z) == 0 && abs(I) == 0 && PC_line >= 10){
      if(option_separateCloud){
        break;
      }
      cout<<"New cloud - nb of lines : "<<PC_line<<endl;
      list_ptxCloud->push_back(cloud);
      cloud = new PTXCloud;
      PC_line = 0;
    }

    this->Loader_header(cloud);
    this->Loader_data(cloud);

    PC_line++;
  }
  list_ptxCloud->push_back(cloud);

  //Scanner at origin
  if(option_scannerAtOrigin){
    this->Loader_scannerAtOrigin();
  }

  //Apply transfomation
  if(option_applyCloudTransfo){
    this->Loader_cloudTransformation();
  }

  //Assemble all clouds
  this->Loader_assembling();

  //---------------------------
  data_out->size = data_out->location.size();
  return data_out;
}
bool file_PTX::Exporter(string path){
  //---------------------------

  //Create file
  if(path.substr(path.find_last_of(".") + 1) != "ptx") path.append(".ptx");
  ofstream file;
  file.open(path);
  if(!file)
  {
    cout<<"Error in creating file !";
    return 0;
  }
  /*
  for(int i=0; i<list_cloud->size(); i++)
  {
    //Select clouds one by one
    Cloud* cloud = &*next(list_cloud->begin(),i);

    //----->HEADER
    //***********************************
    int nbRows = 3;
    if(cloud->subset[0].I.size() != 0) nbRows++;
    if(cloud->subset[0].has_color) nbRows++;
    if(cloud->subset[0].N.size() != 0) nbRows++;
    //number of columns
    file << cloud->subset[0].nb_point << endl;
    //number of rows
    file << nbRows << endl;
    //scanner registered position
    vec3& ScanPos = cloud->scanner.Position;
    file << ScanPos.x << " " << ScanPos.y << " " << ScanPos.z << endl;
    //scanner orientation
    file << 1 << " " << 0 << " " << 0 << endl;
    file << 0 << " " << 1 << " " << 0 << endl;
    file << 0 << " " << 0 << " " << 1 << endl;
    //transformation matrix
    mat4& transMat = cloud->subset[0].trans;
    mat4& rotMat = cloud->subset[0].rotat;
    mat4 finalMat = transpose(transMat * rotMat);
    file << setprecision(6) << finalMat[0][0] << " " << finalMat[0][1] << " " << finalMat[0][2] << " " << finalMat[0][3] << endl;
    file << setprecision(6) << finalMat[1][0] << " " << finalMat[1][1] << " " << finalMat[1][2] << " " << finalMat[1][3] << endl;
    file << setprecision(6) << finalMat[2][0] << " " << finalMat[2][1] << " " << finalMat[2][2] << " " << finalMat[2][3] << endl;
    file << setprecision(6) << finalMat[3][0] << " " << finalMat[3][1] << " " << finalMat[3][2] << " " << finalMat[3][3] << endl;
    //***********************************

    //----->DATA
    //-> Data : xyz (R) (rgb) (nxnynz)

    int precision = 6;

    //Write in the file
    file << pos.size() <<endl;
    for(int i=0; i<pos.size(); i++)
    {
      file << fixed;
      //---> xyz
      file << setprecision(precision) << pos[i].x <<" "<< pos[i].y <<" "<< pos[i].z ;

      //---> R
      if(cloud->subset[0].I.size() != 0)
        file << setprecision(0) <<" "<< ref[i];

      //---> rgb
      //Color only
      if(cloud->subset[0].has_color)
        file << setprecision(0) <<" "<< col[i].x * 255 <<" "<< col[i].y * 255 <<" "<< col[i].z * 255;

      //---> nx ny nz
      if(cloud->subset[0].N.size() != 0)
        file << setprecision(precision) <<" "<< nor[i].x <<" "<< nor[i].y <<" "<< nor[i].z;

      //We end the line
      file << endl;
    }
  }
  */

  //---------------------------
  file.close();
  return true;
}

//Subfunctions
void file_PTX::Loader_header(PTXCloud* cloud){
  //---------------------------

  //Scanner registered position
  if(PC_line == 2){
    cloud->rootTrans.x = x;
    cloud->rootTrans.y = y;
    cloud->rootTrans.z = z;
  }
  //Scanner registered axis
  if(PC_line >= 3 && PC_line <= 5){
    cloud->rootRotat[PC_line-3].x = x;
    cloud->rootRotat[PC_line-3].y = y;
    cloud->rootRotat[PC_line-3].z = z;
  }
  //Transformation matrix
  if(PC_line >= 6 && PC_line <= 9){
    cloud->transfMat[PC_line-6].x = x;
    cloud->transfMat[PC_line-6].y = y;
    cloud->transfMat[PC_line-6].z = z;
    cloud->transfMat[PC_line-6].w = I;
  }

  //---------------------------
}
void file_PTX::Loader_data(PTXCloud* cloud){
  //---------------------------

  if(PC_line > 9){
    if(abs(r) >= 0.0001){
      //Location
      cloud->location.push_back(vec3(x, y, z));

      //Intensity
      if(retrieve_I){
        if(IdataFormat == 0){
          cloud->intensity.push_back(I);
        }else
        if(IdataFormat == 1){
          cloud->intensity.push_back(I/255);
        }else
        if(IdataFormat == 2){
          cloud->intensity.push_back((I+2048)/4096);
        }
      }

      //Color
      if(retrieve_RGB){
        cloud->color.push_back(glm::vec4(r/255, g/255, b/255, 1.0f));
      }
    }
  }

  //---------------------------
}
void file_PTX::Loader_assembling(){
  //Assemble into an unique cloud
  //---------------------------

  for(int i=0; i<list_ptxCloud->size(); i++){
    PTXCloud* cloud = *next(list_ptxCloud->begin(),i);

    for(int j=0; j<cloud->location.size(); j++){
      data_out->location.push_back(cloud->location[j]);
      data_out->intensity.push_back(cloud->intensity[j]);
      data_out->color.push_back(cloud->color[j]);
    }
  }
  //---------------------------
}
void file_PTX::Loader_cloudTransformation(){
  cout<<"---> Apply ptx cloud transformation"<<endl;
  //---------------------------

  for(int i=0; i<list_ptxCloud->size(); i++){
    PTXCloud* cloud = *next(list_ptxCloud->begin(),i);
    vector<vec3>& XYZ = cloud->location;
    mat4& MatT = cloud->transfMat;

    if(option_notUseZValue){
      MatT[3][2] = 0;
    }

    for(int j=0; j<XYZ.size(); j++){
      vec4 pos = vec4(XYZ[j].x, XYZ[j].y, XYZ[j].z, 1.0) * transpose(MatT);
      XYZ[j] = vec3(pos.x, pos.y, pos.z);
    }
  }

  //---------------------------
}
void file_PTX::Loader_scannerAtOrigin(){
  cout<<"---> Set cloud origin at scanner position"<<endl;
  //---------------------------

  for(int i=0; i<list_ptxCloud->size(); i++){
    PTXCloud* cloud = *next(list_ptxCloud->begin(),i);

    vector<vec3>& XYZ = cloud->location;
    vec3& scanTranslation = cloud->rootTrans;
    mat3& scanRotation = cloud->rootRotat;

    if(option_notUseZValue){
      scanTranslation.z = 0;
    }

    for(int j=0; j<XYZ.size(); j++){
      //Translation
      float x = XYZ[j].x + scanTranslation.x;
      float y = XYZ[j].y + scanTranslation.y;
      float z = XYZ[j].z + scanTranslation.z;
      XYZ[j] = vec3(x, y, z);

      //Rotation
      XYZ[j] = XYZ[j] * scanRotation;
    }
  }

  //---------------------------
}
