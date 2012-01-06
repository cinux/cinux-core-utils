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
// 	# gcc -o tcpsniff tcpsniff.c
// 	Dump the executable in the "bin" folder
//
// -----------------------------------------------------------------------------------

// Linux API header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// Cinux API header files
#include "../include/csys.h"

// Declarations
int i, recv_length, sockfd;
u_char buffer[9000];

// Main function
int main(void)
{
   if ((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
   {
      fatal("A fatal error has occured in: socket");
   }

   for(i=0; i < 3; i++)
   {
      recv_length = recv(sockfd, buffer, 8000, 0);
      printf("Got a %d byte packet\n", recv_length);
      dump(buffer, recv_length);
   }
}
