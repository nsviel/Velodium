#ifndef INTERFACE_PREDICTION_H
#define INTERFACE_PREDICTION_H

#include "../../../common.h"

#include <thread>

class Scene;


class Prediction
{
public:
  //Constructor / Destructor
  Prediction();
  ~Prediction();

public:
  //Watchers
  void start_watcher_prediction();
  void stop_watcher_prediction();

  //Subfunctions
  bool runtime_prediction();
  void compute_prediction(Cloud* cloud, string file_path);
  void compute_prediction(Cloud* cloud, vector<string> path_vec);
  void compute_groundTruth(Cloud* cloud, string path_file);
  void compute_groundTruth(Cloud* cloud, vector<string> path_vec);

  //JSON parsers
  void parse_json_groundTruth(Subset* subset, string file_path);
  void parse_json_prediction(Subset* subset, string file_path);
  int parse_frame_ID(string file_path);

  inline string get_path_grThr(){return path_grThr;}
  inline string get_path_predi(){return path_predi;}
  inline bool get_is_watching(){return is_whatching;}
  inline bool* get_is_prediction(){return &is_predictions;}

private:
  Scene* sceneManager;

  string path_dir;
  string path_predi;
  string path_predi_file;
  string path_grThr;
  string path_grThr_file;

  bool thread_predi_ON;
  bool thread_grThr_ON;
  bool is_prediction;
  bool is_whatching;
  bool flag_newPred;
  bool flag_newGrTh;

  std::thread thread_predi;
  std::thread thread_grThr;
};

#endif
