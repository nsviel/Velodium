#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "../../../common.h"


class PCAP_reader
{
public:
  PCAP_reader();
  ~PCAP_reader();

public:
  bool device_get_list();
  bool device_check_name(string name);
  void snif_and_save_pcap();

  inline vector<string> get_devices_name(){return devices_name;}
  inline vector<string> get_devices_desc(){return devices_desc;}
  inline string* get_device_selected(){return &device_selected;}

private:
  vector<string> devices_name;
  vector<string> devices_desc;
  string device_selected;
};

#endif
