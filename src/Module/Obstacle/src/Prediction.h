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
  bool* runtime_prediction();

  //Subfunctions
  void remove_prediction_file(string path);
  void compute_prediction(string file_path);
  void compute_prediction(Cloud* cloud, vector<string> path_vec);
  void compute_groundTruth(Cloud* cloud, string path_file);
  void compute_groundTruth(Cloud* cloud, vector<string> path_vec);

  //JSON parsers
  void parse_json_groundTruth(Subset* subset, string file_path);
  void parse_json_prediction(Subset* subset, string file_path);
  int parse_frame_ID(string file_path);

  inline bool* get_is_new_pred(){return &is_new_pred;}
  inline bool* get_with_delete_pred_file(){return &with_delete_pred_file;}

private:
  Scene* sceneManager;
  Filemanager* fileManager;

  bool is_new_pred;
  bool with_delete_pred_file;
};

#endif
