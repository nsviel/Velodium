#include "file_CSV.h"


//Constructor / Destructor
fileCSV::fileCSV(){}
fileCSV::~fileCSV(){}

//Main function
vector<dataFile*> fileCSV::Loader(string pathFile){
  vector<dataFile*> cloud;
  //---------------------------

  std::ifstream file(pathFile);
  aria::csv::CsvParser parser(file);

  int cpt = 0;
  for (auto& row : parser) {
    dataFile* data = new dataFile();
    data->path = pathFile;

    string tss, xs, ys, zs;
    int cpt = -1;
    for(;;){
      auto field = parser.next_field();

      if(field.type == aria::csv::FieldType::CSV_END){
        break;
      }

      if(field.type != aria::csv::FieldType::ROW_END){
        if(cpt==-1){
          tss = *field.data;
          cpt++;
        }else if(cpt==0){
          xs = *field.data;
          cpt++;
        }
        else if(cpt==1){
          ys =*field.data;
          cpt++;
        }
        else if(cpt==2){
          zs =*field.data;

          float ts = stof(tss);
          float x = stof(xs);
          float y = stof(ys);
          float z = stof(zs);

          vec3 point = vec3(x, y, z);
          data->location.push_back(point);
          data->timestamp.push_back(ts);

          cpt = 0;
        }
      }
      //End of row line
      else{
        cloud.push_back(data);
        data = new dataFile();
        cpt = -1;
      }

    }
  }

  //---------------------------
  return cloud;
}
