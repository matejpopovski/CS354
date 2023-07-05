////////////////////////////////////////////////////////////////////////////////
// Main File:        mySigHandler.c
// This File:        division.c
// Other Files:      mySigHandler.c; sendsig.c
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

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int counter = 0;

/*
 * This function deals with the SIGFPE signal by displaying a 
 * message that indicates the occurrence of a divide-by-zero error, 
 * along with the number of divisions successfully executed until that
 * point. It also provides a final message informing the user that the 
 * program will be terminated.
 *
 */
void handler_SIGFPE() {
  printf("Error: a division by 0 operation was attempted.");
  printf("\nTotal number of operations completed successfully: %d\n", counter);
  printf("The program will be terminated.\n");
  exit(0);
}

/*
 * This is a function that explains what the handler for the 
 * SIGINT signal does. It indicates that when the SIGINT signal is 
 * received, the program will print the number of divisions that have 
 * been successfully completed until that point. Additionally, it will print
 * a final message notifying the user that the program will be 
 * terminated.
 */
void handler_SIGINT() {
  printf("\nTotal number of operations completed successfully: %d\n", counter);
  printf("The program will be terminated.\n");
  exit(0);
}

/*
 * This is the main fuction to perform divisons in. It also handles the SIGFPE and
 * SIGINT handlers.
 *
 */
int main() {
  // Creates new sigaction struct and sets it to 0 with memset
  struct sigaction sig_fpe;
  memset(&sig_fpe, 0, sizeof(sig_fpe));
  // updating handler
  sig_fpe.sa_handler = handler_SIGFPE;

  // bind the SIGFPE signal to handler
  if (sigaction(SIGFPE, &sig_fpe, NULL) != 0) {
    printf("Error binding to signal: SIGFPE\n");
    exit(1);
  }

  // Creates new sigaction struct and sets it to 0 with memset
  struct sigaction sig_int;
  memset(&sig_int, 0, sizeof(sig_int));
  // updating handler
  sig_int.sa_handler = handler_SIGINT;

  // bind the SIGINT signal to handler
  if (sigaction(SIGINT, &sig_int, NULL) != 0) {
    printf("Error binding SIGINT handler\n");
    exit(1);
  }
  int BUFFER = 100;     // buffer for reading input
  char input_1[BUFFER];  // used to read first int value
  char input_2[BUFFER];  // used to read second int value

  // declare variables to be used in the remaining method
  int first_Int, second_Int;
  int result, remainder;

  while (1) {
    // prompt for ints and get the input, convert it to integer

    // standard atoi return value, no checking for non numeric values
    printf("Enter first integer: ");
    fgets(input_1, BUFFER, stdin);
    first_Int = atoi(input_1);

    // standard atoi return value, no checking for non numeric values
    printf("Enter second integer: ");
    fgets(input_2, BUFFER, stdin);
    second_Int = atoi(input_2);

    // compute results for result and remainder
    result = first_Int / second_Int;
    remainder = first_Int % second_Int;

    // display the same
    printf("%d / %d is %d with a remainder of %d\n", first_Int, second_Int,
           result, remainder);
    // increment counter after successful operation
    counter = counter + 1;  
  }

  return 0;
}

