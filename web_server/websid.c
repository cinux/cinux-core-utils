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
// BUILD NOTE:
//
// 	# gcc -o websid websid.c
// 	Dump the executable in the "bin" folder
//
// -----------------------------------------------------------------------------------

// Linux API header files#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
// Cinux API header files
#include "../include/csys.h"
#include "../include/csysnet.h"
#include "inc/websid.h"

// Main function
bool main(int argc, char *argv[])
{
  struct hostent *host_info;
  struct sockaddr_in target_addr;
  
  if(argc < 2)
  {
    printf("Usage   : %s <hostname> [ without \"http://\" in case of website ]\n", argv[0]);
    printf("Example : %s 192.168.1.1\n",     argv[0]); 
    printf("Example : %s www.example.com\n", argv[0]); 
    return false;
  }
  
  if((host_info = gethostbyname(argv[1])) == NULL)
  {
    printf("Error looking up \"%s\"\n", argv[1]);
  }
  
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
  {
    fatal("Error in socket\n");
  }
  
  target_addr.sin_family = AF_INET;
  target_addr.sin_port = htons(80);
  target_addr.sin_addr = *((struct in_addr *)host_info->h_addr);
  memset(&(target_addr.sin_zero), '\0', 8);
  // zero the rest of the struct
  
  if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
  {
    printf("Trying to access \"%s\"\n", argv[1]);
  }
  
  send_string(sockfd, "HEAD / HTTP/1.0\r\n\r\n");
  
  while(recv_line(sockfd, buffer)) 
  {
    if(strncasecmp(buffer, "Server:", 7) == 0)
    {
      printf("The requested webplace's [ \"%s\" ]  server is \"%s\"\n", argv[1], buffer+8);
      return true;
    }
  }
  
  printf("Could not find header on server \"%s\"\n", argv[1]);
  return false;
}