#ifndef INTERFACE_PREDICTION_H
#define INTERFACE_PREDICTION_H

#include "../../../common.h"

#include <thread>

class Module_obstacle;
class Scene;
class Filemanager;


class Prediction
{
public:
  //Constructor / Destructor
  Prediction(Module_obstacle* module);
  ~Prediction();

public:
  //Watchers
  void runtime_prediction();

  //Subfunctions
  void remove_prediction_file(string path);
  void compute_prediction(Cloud* cloud, string file_path);
  void compute_prediction(string path_dir);
  void compute_prediction(Cloud* cloud, vector<string> path_vec);
  void compute_groundTruth(Cloud* cloud, string path_file);
  void compute_groundTruth(Cloud* cloud, vector<string> path_vec);

  //JSON parsers
  void parse_json_groundTruth(Subset* subset, string file_path);
  void parse_json_prediction(Subset* subset, string file_path);
  int parse_frame_ID(string file_path);

  inline bool* get_is_prediction(){return &is_prediction;}
  inline bool* get_with_prediction(){return &with_prediction;}

private:
  Scene* sceneManager;
  Filemanager* fileManager;

  bool is_prediction;
  bool with_prediction;
};

#endif
