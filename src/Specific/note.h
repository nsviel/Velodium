/*





void Prediction::runtime_prediction(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  bool is_prediction = false;
  //---------------------------

  //Load json files - predictions
  if(thread_predi_ON && flag_newPred && cloud != nullptr){
    this->compute_prediction(cloud, path_predi_file);
    this->remove_prediction_file(path_predi_file);
    this->flag_newPred = false;
  }

  //Load json files - GT
  if(thread_grThr_ON && flag_newGrTh && cloud != nullptr){
    this->compute_groundTruth(cloud, path_grThr_file);
    this->flag_newGrTh = false;
  }

  //---------------------------
}
void Prediction::start_watcher_prediction(){
  //---------------------------

  this->thread_predi_ON = true;
  this->thread_grThr_ON = true;

  thread_predi = std::thread([&](){
    while(thread_predi_ON){
      watcher_created_file(".json", path_predi, path_predi_file, flag_newPred);
    }
  });
  thread_grThr = std::thread([&](){
    while(thread_grThr_ON){
      watcher_created_file(".json", path_grThr, path_grThr_file, flag_newGrTh);
    }
  });

  thread_predi.detach();
  thread_grThr.detach();

  this->is_whatching = true;

  //---------------------------
  console.AddLog("ok", "Watcher - Prediction running...");
}
void Prediction::stop_watcher_prediction(){
  //---------------------------

  this->thread_predi_ON = false;
  this->thread_grThr_ON = false;

  thread_predi.~thread();
  thread_grThr.~thread();

  this->is_whatching = false;

  //---------------------------
}


*/
