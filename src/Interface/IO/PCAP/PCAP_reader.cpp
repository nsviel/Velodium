//---------------------------
//Some useful information on https://www.tcpdump.org/pcap.html
//---------------------------
#include "PCAP_reader.h"


//Constructor / Destructor
PCAP_reader::PCAP_reader(){
  //---------------------------

  this->device_selected = "wlp2s0";

  //---------------------------
  this->device_get_list();
}
PCAP_reader::~PCAP_reader(){}

//Device stuff
bool PCAP_reader::device_get_list(){
  bool is_dev_selected = false;
  devices_name.clear();
  //---------------------------

  char errbuf[PCAP_ERRBUF_SIZE] = {0};
  pcap_if_t *ift = NULL;

  if(pcap_findalldevs(&ift, errbuf) == 0) {
    pcap_if_t *it = ift;
    while (it) {
      if(device_check_name(it->name)){
        devices_name.push_back(it->name);
        if(it->name == device_selected){
          is_dev_selected = true;
        }
      }

      it = it->next;
    }
    pcap_freealldevs(ift);
  }
  else {
      printf("error: %s\n", errbuf);
      exit(-1);
  }

  //---------------------------
  return is_dev_selected;
}
bool PCAP_reader::device_check_name(string name){
  //---------------------------

  if(name == "any") return false;
  else if(name == "bluetooth0" || name == "bluetooth-monitor") return false;
  else if(name == "nflog" || name == "nfqueue") return false;
  else if(name == "dbus-system" || name == "dbus-session") return false;
  else if(name == "docker0") return false;

  //---------------------------
  return true;
}

//Sniffing stuff
void PCAP_reader::bind_sniffer(){
  //---------------------------

  // Open capture session
  char errbuf[PCAP_ERRBUF_SIZE];
  this->handle = pcap_open_live(device_selected.c_str(), BUFSIZ, 0, 1500, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", device_selected.c_str(), errbuf);
    return;
  }

  /* Find the properties for the device */
  bpf_u_int32 mask;		/* Our netmask */
  bpf_u_int32 net;		/* Our IP */
	if (pcap_lookupnet(device_selected.c_str(), &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device_selected.c_str(), errbuf);
		net = 0;
		mask = 0;
	}

  /* Compile and apply the filter */
  struct bpf_program fp;		/* The compiled filter */
  char filter_exp[] = "udp port 2370";	/* The filter expression */
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return;
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return;
  }

  //---------------------------
}
void PCAP_reader::unbind_sniffer(){
  //---------------------------

  pcap_close(handle);

  //---------------------------
}
void PCAP_reader::loop_packet(){
  //---------------------------

  struct pcap_pkthdr header;
  const u_char* packet = pcap_next(handle, &header);

  //---------------------------
}

//Saving stuff
void PCAP_reader::open_fileToDump(string path){
  if(path == "") return;
  //---------------------------

  this->dumpfile = pcap_dump_open(handle, path.c_str());

  //---------------------------
}
void PCAP_reader::close_fileToDump(){
  //---------------------------

  pcap_dump_close(dumpfile);

  //---------------------------
}
void PCAP_reader::loop_dump(const u_char* packet){
  //---------------------------

  struct pcap_pkthdr header;
  pcap_dump((unsigned char *)dumpfile, &header, packet);

  //---------------------------
}



void PCAP_reader::snif_and_save_pcap(){
  //---------------------------

  // Define the device
  if(device_get_list() == false) {
    cout << "Selected device not available" << endl;
    return;
  }

  // Open capture session
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* handle = pcap_open_live(device_selected.c_str(), BUFSIZ, 0, 1500, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", device_selected.c_str(), errbuf);
    return;
  }

  /* Find the properties for the device */
  bpf_u_int32 mask;		/* Our netmask */
  bpf_u_int32 net;		/* Our IP */
	if (pcap_lookupnet(device_selected.c_str(), &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", device_selected.c_str(), errbuf);
		net = 0;
		mask = 0;
	}

  /* Compile and apply the filter */
  struct bpf_program fp;		/* The compiled filter */
  char filter_exp[] = "udp port 2370";	/* The filter expression */
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return;
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    return;
  }

  // Open file to save
  string path = "/home/aeter/Desktop/truc.pcap";
  pcap_dumper_t *dumpfile = pcap_dump_open(handle, path.c_str());

  // Capture and save loop
  struct pcap_pkthdr header;	/* The header that pcap gives us */
  const u_char *packet;		/* The actual packet */
  while(1){
    packet = pcap_next(handle, &header);
    pcap_dump((unsigned char *)dumpfile, &header, packet);
  }

  // Close file and session
  pcap_dump_close(dumpfile);
  pcap_close(handle);

//---------------------------
}
