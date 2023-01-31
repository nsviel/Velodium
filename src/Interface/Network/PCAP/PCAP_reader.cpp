//---------------------------
//Some useful information on https://www.tcpdump.org/pcap.html
//---------------------------
#include "PCAP_reader.h"

#include <pcap.h>


//Constructor / Destructor
PCAP_reader::PCAP_reader(){
  //---------------------------

  //---------------------------
}
PCAP_reader::~PCAP_reader(){}

void PCAP_reader::retrieve_device(){
  //---------------------------

  char errbuf[PCAP_ERRBUF_SIZE] = {0};
  pcap_if_t *ift = NULL;

  if(pcap_findalldevs(&ift, errbuf) == 0) {
      pcap_if_t *it = ift;
      while (it) {
          printf("%s - %s\n", it->name, it->description);
          it = it->next;
      }
      pcap_freealldevs(ift);
  }
  else {
      printf("error: %s\n", errbuf);
      exit(-1);
  }

  //---------------------------
}
void PCAP_reader::snif_and_save_pcap(){
  //---------------------------

  // Define the device
  char errbuf[PCAP_ERRBUF_SIZE];
  char* dev = pcap_lookupdev(errbuf);
  if (dev == NULL) {
    fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
    return;
  }

  // Open capture session
  pcap_t* handle = pcap_open_live(dev, BUFSIZ, 0, 1500, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
    return;
  }

  /* Find the properties for the device */
  bpf_u_int32 mask;		/* Our netmask */
  bpf_u_int32 net;		/* Our IP */
	if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1) {
		fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
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
