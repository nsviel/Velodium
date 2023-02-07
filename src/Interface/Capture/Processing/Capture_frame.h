#ifndef CAPTURE_FRAME_H
#define CAPTURE_FRAME_H

#include "../../../Engine/Data/struct_data_cap.h"
#include "../../../common.h"


class Capture_frame
{
public:
  //Constructor / Destructor
  Capture_frame();
  ~Capture_frame();

public:
  //Main functions
  bool build_frame(Data_cap* cloud);
  void reset_frame();

  //Subfunctions
  void add_cloudsToFrame(Data_cap* cloud);
  void end_cloudsToFrame(Data_cap* cloud, int index);

  inline Data_cap* get_endedFrame(){return frame_ended;}

private:
  Data_cap* frame_onrun;
  Data_cap* frame_ended;
};

#endif
