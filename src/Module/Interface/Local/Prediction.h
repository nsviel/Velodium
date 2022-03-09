#ifndef INTERFACE_PREDICTION_H
#define INTERFACE_PREDICTION_H

#include "../../../common.h"

#include <thread>

class Engine_node;
class Scene;


class Prediction
{
public:
  //Constructor / Destructor
  Prediction(Engine_node* node_engine);
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

  inline string get_path_grThr(){return path_grThr;}
  inline string get_path_predi(){return path_predi;}
  inline bool* get_is_prediction(){return &is_prediction;}
  inline bool* get_with_prediction(){return &with_prediction;}

private:
  Scene* sceneManager;

  bool is_prediction;
  bool with_prediction;
  string path_dir;
  string path_predi;
  string path_grThr;
};

#endif
