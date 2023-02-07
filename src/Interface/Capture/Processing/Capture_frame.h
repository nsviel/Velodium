#ifndef CAPTURE_FRAME_H
#define CAPTURE_FRAME_H

#include "../../../Engine/Data/struct_data_udp.h"
#include "../../../common.h"


class Capture_frame
{
public:
  //Constructor / Destructor
  Capture_frame();
  ~Capture_frame();

public:
  //Main functions
  bool build_frame(Data_udp* cloud);
  void reset_frame();

  //Subfunctions
  void add_cloudsToFrame(Data_udp* cloud);
  void end_cloudsToFrame(Data_udp* cloud, int index);

  inline Data_udp* get_endedFrame(){return frame_ended;}

private:
  Data_udp* frame_onrun;
  Data_udp* frame_ended;
};

#endif
