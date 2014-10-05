#include "sniffer.hpp"

pcap_t *handle;         //capture handle
struct bpf_program fp;  //filter program

void printn(const u_char* content, int n)
{
  for(int i=0;i<n;i++)
  {
    printf("%c",content[i]);
  }
}

//print information about TCP package
void printTCP(u_char* content, bpf_u_int32 length)
{
  struct tcp_header_t* tcp=(struct tcp_header_t*) content;
  printf(" TCP\n Ports:\n");
  printf("  src: %i\n",ntohs(tcp->src_port));
  printf("  dst: %i\n",ntohs(tcp->dst_port));
  int offset=(tcp->data_offset>>4)<<2;
  printf(" Content(%i):\n",length-offset);
  printn(content+offset,length-offset); printf("\n");
}

//print information about IP package
void printIPPackage(u_char* content, bpf_u_int32 length)
{
  content=content+SIZE_ETHERNET;
  length=length-SIZE_ETHERNET;
  struct ip_header* iph = (struct ip_header*) (content);
  char ip_buffer[16];
  int i;
  printf("Package Information:\n");
  //print source
  unsigned char* addr=(unsigned char*) &iph->ip_src;
  sprintf(ip_buffer,"%i.%i.%i.%i",addr[0],addr[1],addr[2],addr[3]);
  printf(" From: %s   ",ip_buffer);
  for(i=strlen(ip_buffer);i<16;i++)
    printf(" ");
  //print destination
  addr=(unsigned char*) &iph->ip_dst;
  sprintf(ip_buffer,"%i.%i.%i.%i",addr[0],addr[1],addr[2],addr[3]);
  printf("To: %s\n",ip_buffer);
  switch(iph->ip_p)
  {
    case 1: //ICMP
      printf(" ICMP\n");
      break;
    case 6: //TCP
      printTCP(content+sizeof(struct ip_header),length-sizeof(struct ip_header));
      break;
    case 11: //UDP
      printf(" UDP\n");
      break;
    default:
      printf("Unknown protocol (%i)\n",iph->ip_p);
  }
}

void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet)
{
  packet=packet;
  struct ip_header* ip = (struct ip_header*)(packet + SIZE_ETHERNET);
  int direction=0;
  unsigned int ip_src=ip->ip_src;
  if( isInSubnet(ip_src) ) 
  {
    direction=1;
    ip_src=ip->ip_dst;
  }
  ip_src=((ip_src&0x000000FF)<<24) | ((ip_src&0x0000FF00)<<8) | ((ip_src&0x00FF0000)>>8) | ((ip_src&0xFF000000)>>24);
  glm::vec3 color;
  color.x=0.5*sin(2*M_PI*(ip_src/4294967296.0))+0.5;
  color.y=0.5*sin(2*M_PI*(ip_src/4294967296.0+0.333))+0.5;
  color.z=0.5*sin(2*M_PI*(ip_src/4294967296.0+0.666))+0.5;

  glm::vec3 scale(header->len/1500.0+0.15);
  addNewNetworkPackage(direction,color,scale,packet,header->len);
}

void* sniffer_start_thread(void* arg)
{
  while(1)
    pcap_loop(handle, 0, got_packet, NULL);
  pcap_freecode(&fp);
  pcap_close(handle);
  return NULL;
}

void sniffer_start()
{
  char *dev = NULL;              //capture device name
  char errbuf[PCAP_ERRBUF_SIZE]; //pcap error buffer

  char filter_exp[] = "ip"; //filter expression
  bpf_u_int32 mask;         //subnet mask
  bpf_u_int32 net;          //ip address

  //pick device
  dev = pcap_lookupdev(errbuf);
  if (dev == NULL)
  {
    fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
    exit(EXIT_FAILURE);
  }

  //get information on the device
  if (pcap_lookupnet(dev, &net, &mask, errbuf) == -1)
  {
    fprintf(stderr, "Couldn't get netmask for device %s: %s\n", dev, errbuf);
    net = 0;
    mask = 0;
  }
  printf("Device: %s\n", dev);

  //define the local network for visualisation
  defineNetwork(net,mask);

  //open capture divice
  handle = pcap_open_live(dev, SNAP_LEN, 1, 300, errbuf);
  if (handle == NULL)
  {
    fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
    exit(EXIT_FAILURE);
  }

  //check if device is ethernet
  if (pcap_datalink(handle) != DLT_EN10MB)
  {
    fprintf(stderr, "%s is not an Ethernet\n", dev);
    exit(EXIT_FAILURE);
  }

  //compile filter program
  if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1)
  {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }

  //apply filter
  if (pcap_setfilter(handle, &fp) == -1)
  {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
    exit(EXIT_FAILURE);
  }
  //start sniffer thread
  pthread_t thread;
  pthread_create(&thread,NULL,sniffer_start_thread,&handle);
  pthread_detach(thread);
}

