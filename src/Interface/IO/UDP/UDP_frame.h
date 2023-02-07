#ifndef UDP_FRAME_H
#define UDP_FRAME_H

#include "../../../Engine/Data/struct_UDPpacket.h"
#include "../../../common.h"


class UDP_frame
{
public:
  //Constructor / Destructor
  UDP_frame();
  ~UDP_frame();

public:
  //Main functions
  bool build_frame(udpPacket* cloud);
  void reset_frame();

  //Subfunctions
  void add_cloudsToFrame(udpPacket* cloud);
  void end_cloudsToFrame(udpPacket* cloud, int index);

  inline udpPacket* get_endedFrame(){return frame_ended;}

private:
  udpPacket* frame_onrun;
  udpPacket* frame_ended;
};

#endif
