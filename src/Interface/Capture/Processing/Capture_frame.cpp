#include "Capture_frame.h"


//Constructor / Destructor
Capture_frame::Capture_frame(){
  //---------------------------

  this->frame_onrun = new Data_cap();
  this->frame_ended = new Data_cap();

  //---------------------------
}
Capture_frame::~Capture_frame(){}

//Main function
bool Capture_frame::build_frame(Data_cap* data_udp){
  bool frame_ended = false;
  //---------------------------

  //check if the new cloud begin by a revolution
  // frame_index -1: new frame
  // frame_index 0: frame end and restart at 0
  // frame_index i: frame end at point i
  if(data_udp->A.size() != 0){
    //First case: no previous frame created
    int frame_index = -1;

    //Second case: the packet is the first of a frame, but the previousframe should have 10k pts
    if(frame_onrun->A.size() > 10000){
      if(data_udp->A[0] + 350 < frame_onrun->A[frame_onrun->A.size()-1] ){
        frame_index = 0;
      }
    }
    //Third case: we search if or not the end of the frame in inside the packet
    //This happen rarely
    else{
      for(int i=0; i<data_udp->A.size() - 1; i++){
        if( data_udp->A[i+1] < data_udp->A[i] ){
          frame_index = i;
          say("thats happen !");
          break;
        }
      }
    }

    //say(data_udp->A[data_udp->A.size()-1]);
    //Then: first case or no index found
    if(frame_index == -1){
      this->add_cloudsToFrame(data_udp);
      frame_ended = false;
    }
    //Then: second and third cases where an end index was found
    else{
      this->end_cloudsToFrame(data_udp, frame_index);
      frame_ended = true;
    }
  }

  //---------------------------
  delete data_udp;
  return frame_ended;
}
void Capture_frame::reset_frame(){
  //---------------------------

  delete frame_onrun;
  delete frame_ended;

  this->frame_onrun = new Data_cap();
  this->frame_ended = new Data_cap();

  //---------------------------
}

//Subfunctions
void Capture_frame::add_cloudsToFrame(Data_cap* data_udp){
  //---------------------------

  for(int i=0; i<data_udp->xyz.size(); i++){
    frame_onrun->xyz.push_back(data_udp->xyz[i]);
    frame_onrun->R.push_back(data_udp->R[i]);
    frame_onrun->I.push_back(data_udp->I[i]/255);
    frame_onrun->A.push_back(data_udp->A[i]);
    frame_onrun->t.push_back(data_udp->t[i]);
  }

  //---------------------------
}
void Capture_frame::end_cloudsToFrame(Data_cap* data_udp, int index){
  //---------------------------

  for(int i=0; i<index; i++){
    frame_onrun->xyz.push_back(data_udp->xyz[i]);
    frame_onrun->R.push_back(data_udp->R[i]);
    frame_onrun->I.push_back(data_udp->I[i]/255);
    frame_onrun->A.push_back(data_udp->A[i]);
    frame_onrun->t.push_back(data_udp->t[i]);
  }

  *frame_ended = *frame_onrun;
  delete frame_onrun;
  frame_onrun = new Data_cap();

  for(int i=index; i<data_udp->xyz.size(); i++){
    frame_onrun->xyz.push_back(data_udp->xyz[i]);
    frame_onrun->R.push_back(data_udp->R[i]);
    frame_onrun->I.push_back(data_udp->I[i]/255);
    frame_onrun->A.push_back(data_udp->A[i]);
    frame_onrun->t.push_back(data_udp->t[i]);
  }

  //---------------------------
}
