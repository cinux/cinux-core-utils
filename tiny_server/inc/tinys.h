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

#ifndef TINYS_H
#define TINYS_H

#define PORT 80   			 // The port users will be connecting to 
#define WEBROOT "/usr/share/webroot"	 // The web server's root directory 
					 // This file needs to be installed in 
					 // /usr/share

// From main.c
#define LOGFILE "/var/log/tinys.log"	 // Log filename 

// Define some functions   
int logfd, sockfd;  			// Global log and socket file descriptor variables
void handle_connection
     (int, struct sockaddr_in *, int);  // This method litterally does what it's name sais
int get_file_size(int); 		// This method Returns the filesize of open file descriptor 
void timestamp(int); 			// This method writes a timestamp to the open file descriptor 

// From sclient.c

void Fhandle_connection(int, struct sockaddr_in *); // handle web requests
int Fget_file_size(int); // returns the filesize of open file descriptor


#endif