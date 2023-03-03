#include "file_LAS.h"
#include "../../Specific/File/Info.h"

#include <fstream>


//Constructor / Destructor
file_LAS::file_LAS(){}
file_LAS::~file_LAS(){}

//Main function
Data_file* file_LAS::Loader(string path){
  Data_file* data = new Data_file();
  data->name = get_name_from_path(path);
  data->path_file = path;
  //---------------------------

  this->read(path);

  //---------------------------
  return data;
}



struct Header{
    char magic[4];
    uint16_t fileSourceID;
    uint16_t globalEncoding;
    uint32_t guidData1;
    uint16_t guidData2;
    uint16_t guidData3;
    uint8_t guidData4[8];
    uint8_t versionMaj, versionMin;
    char systemIdentifier[32];
    char genSoftware[32];
    uint16_t creationDay, creationYear;
    uint16_t headerSize;
    uint32_t pointDataOffset;
    uint32_t numVarLenRecords;
    uint8_t pointDataRecordFormat;
    uint16_t pointDataRecordLen;
    uint32_t numberOfPoints;
    uint32_t numPointsByReturn[5];
    double scaleX, scaleY, scaleZ;
    double offsetX, offsetY, offsetZ;
    double maxX, minX, maxY, minY, maxZ, minZ;
    uint64_t waveform;
};
struct PointRecord3{
    uint32_t x, y, z;
    uint16_t intensity;
    uint8_t flags;
    uint8_t classification;
    uint8_t scanAngleRank;
    uint8_t userData;
    uint16_t pointSourceId;
    double gpsTime;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

void file_LAS::read(string path){
  //---------------------------

  ifstream file(path, ios::binary);

  if (file.is_open()){
    Header header;
    file.read((char*)&header, sizeof(header));

    cout << "Signature: " << header.magic << endl;
    cout << "Source ID: " << int(header.fileSourceID) << endl;
    cout << "Global Encoding: " << int(header.globalEncoding) << endl;
    cout << "Guid 1: " << int(header.guidData1) << endl;
    cout << "Guid 2: " << int(header.guidData2) << endl;
    cout << "Guid 3: " << int(header.guidData3) << endl;
    cout << "Guid 4: " << header.guidData4 << endl;
    cout << (int)header.versionMaj << '.' << (int)header.versionMin << endl;
    cout << "Sys Identifier: " << header.systemIdentifier << endl;
    cout << "Gen Software: " << header.genSoftware << endl;
    cout << "Creation Day: " << header.creationDay << endl;
    cout << "Creation Year: " << header.creationYear << endl;
    cout << header.headerSize << " == " << sizeof(header) << endl;
    cout << "Point Data Offset: " << header.pointDataOffset << endl;
    cout << "Number of Variable Len Records: " << header.numVarLenRecords << endl;
    cout << "point Data Record Format: " << header.pointDataRecordFormat << endl;
    cout << "point Data Record Len: " << header.pointDataRecordLen << endl;
    cout << "Number of Points: " << header.numberOfPoints << endl;
    cout << "Number of Points by Return: " << header.numPointsByReturn << endl;
    cout << "Scales: " << header.scaleX << ", " << header.scaleY << ", " << header.scaleZ << endl;
    cout << "Offsets: " << header.offsetX << ", " << header.offsetY << ", " << header.offsetZ << endl;
    cout << "Xmin = " << header.minX << ", Ymin = " << header.minY << ", Zmin = " << header.minZ << endl;
    cout << "Xmax = " << header.maxX << ", Ymax = " << header.maxY << ", Zmax = " << header.maxZ << endl;
    cout << "Waveform: "<<header.waveform << endl;

    assert(header.versionMaj == 1 && header.versionMin == 3);
    assert(header.pointDataRecordFormat == 3);

    file.seekg(sizeof(header));
    for (uint32_t i = 0; i < header.numberOfPoints; i++){
      PointRecord3 point;
      file.read((char*)&point, sizeof(PointRecord3));

      float x = (float)(point.x * header.scaleX) + header.offsetX;
      float y = (float)(point.y * header.scaleY) + header.offsetY;
      float z = (float)(point.z * header.scaleZ) + header.offsetZ;
      float intensity = (float)(point.intensity) / 65536.0;
    }
  }

  //---------------------------
  //std::cout << "Cloud size = " << cloud->points.size() << endl;
  //return cloud;
}
