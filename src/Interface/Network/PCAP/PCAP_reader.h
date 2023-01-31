#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "../../../common.h"


class PCAP_reader
{
public:
  PCAP_reader();
  ~PCAP_reader();

public:
  void retrieve_device();
  void snif_and_save_pcap();

private:

};

#endif
