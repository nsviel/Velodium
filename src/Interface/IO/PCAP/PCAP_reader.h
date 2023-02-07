#ifndef PCAP_READER_H
#define PCAP_READER_H

#include "../../../common.h"

#include <pcap.h>


class PCAP_reader
{
public:
  PCAP_reader();
  ~PCAP_reader();

public:
  //Device stuff
  bool device_get_list();
  bool device_check_name(string name);

  //Sniffing stuff
  void bind_sniffer();
  void unbind_sniffer();
  void loop_packet();

  //Saving stuff
  void open_fileToDump(string path);
  void close_fileToDump();
  void loop_dump(const u_char* packet);

  void snif_and_save_pcap();

  inline vector<string> get_devices_name(){return devices_name;}
  inline vector<string> get_devices_desc(){return devices_desc;}
  inline string* get_device_selected(){return &device_selected;}
  inline bool* get_snif_and_save(){return &snif_and_save;}

private:
  //Normal stuff
  vector<string> devices_name;
  vector<string> devices_desc;
  string device_selected;
  bool snif_and_save;

  //Library stuff
  pcap_t* handle;
  pcap_dumper_t* dumpfile;
};

#endif
