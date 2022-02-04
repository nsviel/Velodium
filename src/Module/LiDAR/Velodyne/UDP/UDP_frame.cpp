#include "UDP_frame.h"


//Constructor / Destructor
UDP_frame::UDP_frame(){
  //---------------------------

  frame_onrun = new udpPacket();

  //---------------------------
}
UDP_frame::~UDP_frame(){}

//Main function
bool UDP_frame::build_frame(udpPacket* cloud){
  //---------------------------

  //check if the new cloud begin by a revolution
  // index_frame -1: new frame
  // index_frame 0: frame end and restart at 0
  // index_frame i: frame end at point i
  if(cloud->A.size() == 0){
    return false;
  }

  //First case: no previous frame created
  int index_frame = -1;

  //Second case: the packet is the first of a frame, but the previousframe should have 10k pts
  if(frame_onrun->A.size() != 0 && frame_onrun->A.size() > 10000){
    if(cloud->A[0] + 350 < frame_onrun->A[frame_onrun->A.size()-1] ){
      index_frame = 0;
    }
  }
  //Third case: we search if or not the end of the frame in inside the packet
  //This happen rarely
  else{
    for(int i=0; i<cloud->A.size() - 1; i++){
      if( cloud->A[i+1] < cloud->A[i] ){
        index_frame = i;
        break;
      }
    }
  }

  //Then: first case or no index found
  if(index_frame == -1){
    this->add_cloudsToFrame(cloud);
    return false;
  }
  //Then: second and third cases where an end index was found
  else{
    this->end_cloudsToFrame(cloud, index_frame);

    return true;
  }

  //---------------------------
}

//Subfunctions
void UDP_frame::add_cloudsToFrame(udpPacket* cloud){
  //---------------------------

  for(int i=0; i<cloud->xyz.size(); i++){
    frame_onrun->xyz.push_back(cloud->xyz[i]);
    frame_onrun->R.push_back(cloud->R[i]);
    frame_onrun->I.push_back(cloud->I[i]/255);
    frame_onrun->A.push_back(cloud->A[i]);
    frame_onrun->t.push_back(cloud->t[i]);
  }

  //---------------------------
}
void UDP_frame::end_cloudsToFrame(udpPacket* cloud, int index){
  //---------------------------

  for(int i=0; i<index; i++){
    frame_onrun->xyz.push_back(cloud->xyz[i]);
    frame_onrun->R.push_back(cloud->R[i]);
    frame_onrun->I.push_back(cloud->I[i]/255);
    frame_onrun->A.push_back(cloud->A[i]);
    frame_onrun->t.push_back(cloud->t[i]);
  }

  frame_ended = frame_onrun;
  frame_onrun = new udpPacket();

  for(int i=index; i<cloud->xyz.size(); i++){
    frame_onrun->xyz.push_back(cloud->xyz[i]);
    frame_onrun->R.push_back(cloud->R[i]);
    frame_onrun->I.push_back(cloud->I[i]/255);
    frame_onrun->A.push_back(cloud->A[i]);
    frame_onrun->t.push_back(cloud->t[i]);
  }

  //---------------------------
}
