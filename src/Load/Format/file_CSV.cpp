#include "file_CSV.h"

#include "../CSV/CSV_state.h"

#include <fstream>


//Constructor / Destructor
file_CSV::file_CSV(){}
file_CSV::~file_CSV(){}

//Main function
vector<dataFile*> file_CSV::Loader(string pathFile){
  vector<dataFile*> cloud;
  //---------------------------

  std::ifstream file(pathFile);
  std::vector<std::vector<std::string>> csvFile = readCSV(file);

  //At row level
  for(int i=0; i<csvFile.size(); i++){
    dataFile* data = new dataFile();
    data->path = pathFile;

    //At field level
    int cpt_field = -1;
    int cpt_point = 0;
    string ts_str, x_str, y_str, z_str;
    for(int j=0; j<csvFile[i].size(); j++){
      string field = csvFile[i][j];

      if(j == 0 && cpt_field == -1){
        ts_str = field;
        cpt_field++;
      }
      else if(cpt_field == 0){
        x_str = field;
        cpt_field++;
      }
      else if(cpt_field == 1){
        y_str = field;
        cpt_field++;
      }
      else if(cpt_field == 2){
        z_str = field;

        float ts = stof(ts_str);
        float x = stof(x_str);
        float y = stof(y_str);
        float z = stof(z_str);

        //Relative timestamp
        float delay = 1000000.0f / (25.0f * 340.0f);
        ts = ts + cpt_point * delay;

        vec3 point = vec3(x, y, z);

        data->name = "frame_" + to_string(i);
        data->timestamp.push_back(ts);
        data->location.push_back(point);

        cpt_field = 0;
        cpt_point++;
      }
    }

    //End of row line
    cloud.push_back(data);
  }

  //---------------------------
  return cloud;
}
