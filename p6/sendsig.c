////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        sendsig.c
// Other Files:      division.c; mySigHandler.c
// Semester:         CS 354 Spring 2023
// Instructor:       deppeler
//
// Author:           Matej Popovski
// Email:	     popovski@wisc.edu
// CS Login:	     matej
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Copyright 2013,2019-2020
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission for Spring 2023
//
////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

/**
* This is a function to send SIGINT
*
* param - pid, PID to interrupt
*/
void SIGINT_SEND(int pid) {
	int temp1 = 0;
	temp1 = kill(pid, SIGINT);

	//checking if kill() returned null
	if (temp1 < 0 ) {
		printf("error: unable to send SIGINT\n");
	}
}

/**
* This is a function to send SIGINT
*
* param - pid, PID to send the SIGUSR1 to
*/
void SIGUSR1_SEND(int pid) {
	int temp1 = 0;
	temp1 = kill(pid, SIGUSR1);

	//checking if kill() returned null
	if (temp1 < 0 ) {
		printf("error: unable to send SIGUSR1\n");
	}
	exit(0);
}

/**
* This is the main function
* It checks user input if it matches requirements then calls the proper sender functions
*/
int main(int argc, char **argv) {

	int pid = atoi(argv[2]);


	if (pid < 0) {
		printf("invalid PID, PID cannot be negative.\n");
	}

	/**
	* checks if number of arguments  is correct
	* checks if the command inputted is correct (-i or -u)
	*/
	if (argc == 2 || strcmp(argv[1], "-i") != 0|| strcmp(argv[1], "-u") != 0) {
		if (strcmp(argv[1], "-i") == 0) {
			SIGINT_SEND(pid);
		} else if (strcmp(argv[1], "-u") == 0) {
			SIGUSR1_SEND(pid);
		}
	} else {
		printf("Usage: sendsig <signal type> <pid>\n");
	}
}