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

#ifndef CSYS_H
#define CSYS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Show this message just to be typical with the fucking exploiters or hackers.
#define DISCLAIMER									\
{											\
   printf("Remember: \nThe software contained in Cinux Core Utils "			\
	"may be illegal to be used in certain ways and regions. All programs "		\
	"have been assembed and brought to you for educational purposes. Users "	\
	"use these programs ON THEIR OWN RISCK. Neither Cinux nor its creator "		\
	"are responsible for how they are used. If you get in trouble, you are "	\
	"on your own. \nThank you for your understanding\n");}	

// A function to display an error message and then exit
void fatal(char *message)
{
   char error_message[100];

   strcpy(error_message, "[!!] ");
   strncat(error_message, message, 83);
   perror(error_message);
   exit(-1);
}

// An error checked malloc() wrapper function
void *ec_malloc(unsigned int size)
{
   void *ptr;
   ptr = malloc(size);
   if(ptr == NULL)
   {
      fatal("Error in ec_malloc() on memory allocation");
   }
   return ptr;
}

// dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) 
{
  unsigned char byte;
  unsigned int i, j;
  for(i=0; i < length; i++) 
  {
    byte = data_buffer[i];
    printf("%02x ", data_buffer[i]);
    // display byte in hex
    if(((i%16)==15) || (i==length-1))
    {
      for(j=0; j < 15-(i%16); j++)
      {
	printf("   ");
      }
      printf("| ");
      for(j=(i-(i%16)); j <= i; j++) 
      { 
	// display printable bytes from line
	byte = data_buffer[j];
	if((byte > 31) && (byte < 127))
	// outside printable char range
	{
	  printf("%c", byte);
	}
	else
	{
	  printf(".");
	}
      }
      printf("\n");
      // end of the dump line (each line 16 bytes)
    }
  }
}

#endif
