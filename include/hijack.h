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

#ifndef HIJACK_H
#define HIJACK_H

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);
int set_packet_filter(pcap_t *, struct in_addr *);

struct data_pass {
   int libnet_handle;
   u_char *packet;
};


#endif