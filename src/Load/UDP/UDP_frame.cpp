#include "UDP_frame.h"


//Constructor / Destructor
UDP_frame::UDP_frame(){
  //---------------------------

  this->frame_onrun = new udpPacket();
  this->frame_ended = new udpPacket();

  //---------------------------
}
UDP_frame::~UDP_frame(){}

//Main function
bool UDP_frame::build_frame(udpPacket* packet_udp){
  bool frame_ended = false;
  //---------------------------

  //check if the new cloud begin by a revolution
  // frame_index -1: new frame
  // frame_index 0: frame end and restart at 0
  // frame_index i: frame end at point i
  if(packet_udp->A.size() != 0){
    //First case: no previous frame created
    int frame_index = -1;

    //Second case: the packet is the first of a frame, but the previousframe should have 10k pts
    if(frame_onrun->A.size() != 0 && frame_onrun->A.size() > 10000){
      if(packet_udp->A[0] + 350 < frame_onrun->A[frame_onrun->A.size()-1] ){
        frame_index = 0;
      }
    }
    //Third case: we search if or not the end of the frame in inside the packet
    //This happen rarely
    else{
      for(int i=0; i<packet_udp->A.size() - 1; i++){
        if( packet_udp->A[i+1] < packet_udp->A[i] ){
          frame_index = i;
          break;
        }
      }
    }

    //Then: first case or no index found
    if(frame_index == -1){
      this->add_cloudsToFrame(packet_udp);
      frame_ended = false;
    }
    //Then: second and third cases where an end index was found
    else{
      this->end_cloudsToFrame(packet_udp, frame_index);
      frame_ended = true;
    }
  }

  //---------------------------
  delete packet_udp;
  return frame_ended;
}
void UDP_frame::reset_frame(){
  //---------------------------

  delete frame_onrun;
  delete frame_ended;

  this->frame_onrun = new udpPacket();
  this->frame_ended = new udpPacket();

  //---------------------------
}

//Subfunctions
void UDP_frame::add_cloudsToFrame(udpPacket* packet_udp){
  //---------------------------

  for(int i=0; i<packet_udp->xyz.size(); i++){
    frame_onrun->xyz.push_back(packet_udp->xyz[i]);
    frame_onrun->R.push_back(packet_udp->R[i]);
    frame_onrun->I.push_back(packet_udp->I[i]/255);
    frame_onrun->A.push_back(packet_udp->A[i]);
    frame_onrun->t.push_back(packet_udp->t[i]);
  }

  //---------------------------
}
void UDP_frame::end_cloudsToFrame(udpPacket* packet_udp, int index){
  //---------------------------

  for(int i=0; i<index; i++){
    frame_onrun->xyz.push_back(packet_udp->xyz[i]);
    frame_onrun->R.push_back(packet_udp->R[i]);
    frame_onrun->I.push_back(packet_udp->I[i]/255);
    frame_onrun->A.push_back(packet_udp->A[i]);
    frame_onrun->t.push_back(packet_udp->t[i]);
  }

  *frame_ended = *frame_onrun;
  delete frame_onrun;
  frame_onrun = new udpPacket();

  for(int i=index; i<packet_udp->xyz.size(); i++){
    frame_onrun->xyz.push_back(packet_udp->xyz[i]);
    frame_onrun->R.push_back(packet_udp->R[i]);
    frame_onrun->I.push_back(packet_udp->I[i]/255);
    frame_onrun->A.push_back(packet_udp->A[i]);
    frame_onrun->t.push_back(packet_udp->t[i]);
  }

  //---------------------------
}
