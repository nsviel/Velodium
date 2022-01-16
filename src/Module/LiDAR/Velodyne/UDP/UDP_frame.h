#ifndef UDP_frame_H
#define UDP_frame_H

#include "../../../../common.h"
#include "struct_UDPpacket.h"


class UDP_frame
{
public:
  //Constructor / Destructor
  UDP_frame();
  ~UDP_frame();

public:
  //Main functions
  bool build_frame(udpPacket* cloud);

  //Subfunctions
  void add_cloudsToFrame(udpPacket* cloud);
  void end_cloudsToFrame(udpPacket* cloud, int index);

  inline udpPacket* get_endedFrame(){return frame_ended;}

private:
  udpPacket* frame_onrun;
  udpPacket* frame_ended;
};

#endif
