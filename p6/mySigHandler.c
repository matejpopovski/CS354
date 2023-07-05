////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        mySigHandler.c
// Other Files:      division.c; sendsig.c
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
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

//to count how many times SIGUSR1 was handled
int counter = 0;

/**
* Function that handles SIGALRM.
* Checks time and sets it if not. 
* prints out PID and current time every 4 seconds.
*/
void SIGALRM_handler() {
	time_t time1 = time(NULL);

	//checks return value of time()
	if (!time1) {
		printf("error: cannot get time\n");
		exit(1);
	}

	char *time2 = ctime(&time1);

	//checks return value of ctime()
	if (!time2) {
		printf("error: cannot format time\n");
	}

	printf("PID: %d CURRENT TIME: %s", getpid(), time2);
	alarm(4);
}
/**
* The function handles the SIGUSR1 
* signal. When the signal is received, the function performs two tasks: 
* printing a message and incrementing a counter to keep track of how 
* many times the signal has been handled.
*/
void SIGUSR1_handler() {
	printf("SIGUSR1 handled and counted!\n");
	counter++;
}

/**
* This function handles SIGINT
* It prints out a message with how many times SIGUSR1 was handled, then exits
*/
void SIGINT_handler() {
	printf("\nSIGINT handled.\nSIGUSR1 was handled %d times. Exiting now.\n", counter);
	exit(0);
}

/**
* This is the main function
* It sets off an alarm by using an infinite loop
* Is declares and initializes signal handlers
*/
int main () {

	alarm(4);
	printf("PID and time print every 4 seconds.\n Type Ctrl-C to end the program.\n");

	//This is a struct for SIGINT
	struct sigaction act1;
	memset(&act1, 0, sizeof(act1));
	act1.sa_handler = SIGALRM_handler;
	
	//This is a struct for SIGUSR1
	struct sigaction act2;
	memset(&act2, 0, sizeof(act2));
	act2.sa_handler = SIGUSR1_handler;

	//This is a struct for SIGINT
	struct sigaction act3;
	memset(&act3, 0, sizeof(act3));
	act3.sa_handler = SIGINT_handler;

	if (sigaction(SIGALRM, &act1, NULL) != 0) {
		printf("error: binding SIGALRM handler\n");
		exit(1);
	}

	if (sigaction(SIGUSR1, &act2, NULL) != 0) {
		printf("error: binding SIGUSR1 handler\n");
		exit(1);
	}

	if (sigaction(SIGINT, &act3, NULL) != 0) {
		printf("error: binding SIGINT handler\n");
		exit(1);
	}

	while(1) { }
	return 0;
}