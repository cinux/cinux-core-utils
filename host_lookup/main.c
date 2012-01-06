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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
// Cinux API header files
#include "../include/csys.h" 
#include "../include/csysnet.h" 

// Declarations
struct hostent *host_info;
struct in_addr *address;

// Main function
int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    // Print the usage along with two examples
    printf("Usage   : %s <hostname>\n", argv[0]);
    printf("Example : %s 192.168.1.1\n", argv[0]);
    printf("Example : %s www.example.com\n", argv[0]);
    exit(1);
  }
  
  host_info = gethostbyname(argv[1]);
  if(host_info == NULL)
  {
    printf("Couldn't lookup \"%s\"\n", argv[1]);
  } 
  else
  {
    address = (struct in_addr *) (host_info->h_addr);
    printf("\"%s\" has address \"%s\"\n", argv[1], inet_ntoa(*address));
  }
}
