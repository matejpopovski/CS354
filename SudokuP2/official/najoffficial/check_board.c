///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        check_board.c
// This File:        check_board.c
// Other Files:      (name of all other files if any)
// Semester:         CS 354 Spring 2023
// Instructor:       Debra Deppeler
//
// Author:           Matej Popovski
// Email:            popovski@wisc.edu
// CS Login:         matej
// GG#:              GG1
//                   (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   including family, friencs, classmates, tutors,
//                   Peer Mentors, TAs, and Instructor.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *DELIM_DELIMITER = ",";

/* COMPLETED (DO NOT EDIT):       
 * Read the first line of input file to get the size of that board.
 * 
 * PRE-CONDITION #1: file exists
 * PRE-CONDITION #2: first line of file contains valid non-zero integer value
 *
 * fptr: file pointer for the board's input file
 * size: a pointer to an int to store the size
 *
 * POST-CONDITION: the integer whos address is passed in as size (int *) 
 * will now have the size (number of rows and cols) of the board being checked.
 */
void get_board_size(FILE *fptr, int *size) {      
	char *line1 = NULL;
	size_t len = 0;

	if ( getline(&line1, &len, fptr) == -1 ) {
		printf("invalid\n");
		free(line1);
		exit(1);
	}

	char *size_chars = NULL;
	size_chars = strtok(line1, DELIM_DELIMITER);
	*size = atoi(size_chars);

	free(line1);
	line1 = NULL;
}



/* TODO:
 * Returns 1 if and only if the board is in a valid Sudoku board state.
 * Otherwise returns 0.
 * 
 * A valid row or column contains only blanks or the digits 1-size, 
 * with no duplicate digits, where size is the value 1 to 9.
 * 
 * Note: p2A requires only that each row and each column are valid.
 * 
 * board: heap allocated 2D array of integers 
 * size:  number of rows and columns in the board
 */
int valid_board(int **board, int size) {
	int returnedValue = 1;
    for (int a = 0; a < size; a++){
	    for(int b = 0; b < size; b++){
		 
		    if (*(*(board+a)+b) < 0 || *(*(board+a)+b) > size) {
			    returnedValue = 0;
			   
		    }

		   
		    for (int i = b+1; i < size; i++){
			    if (*(*(board+a)+b) != 0 && *(*(board+a)+b) == *(*(board+a)+i)){
				    returnedValue = 0;
				    

			    }
		    }						 

		    
		    for (int c = b+1;  c < size; c++){
			    if (*(*(board+b)+a) != 0 && *(*(board+b)+a) == *(*(board+c)+a)){
				    returnedValue = 0;
				  

			    }
		    }
	    }
    }

    return returnedValue;     
}    



/* TODO: COMPLETE THE MAIN FUNCTION
 * This program prints "valid" (without quotes) if the input file contains
 * a valid state of a Sudoku puzzle board wrt to rows and columns only.
 *
 * A single CLA is required, which is the name of the file 
 * that contains board data is required.
 *
 * argc: the number of command line args (CLAs)
 * argv: the CLA strings, includes the program name
 *
 * Returns 0 if able to correctly output valid or invalid.
 * Only exit with a non-zero result if unable to open and read the file given.
 */
int main( int argc, char **argv ) {              

	
    if (argc != 2){
	    printf("Usage: ./check_board <input_filename>\n");
	    exit(1);
    }
    
	
	FILE *fp = fopen(*(argv + 1), "r");
	if (fp == NULL) {
		printf("invalid\n");
		exit(1);
	}

	
	int size;
    
	
    get_board_size(fp, &size);
    
    if (size < 1 || size > 9){
	    printf("invalid\n");
	    exit(1);
    }
    
	
    int **board = malloc(sizeof(int*) * size);
    
    if(board == NULL){
            printf("invalid\n");
            exit(1);
    } 

    for (int i = 0; i < size; i++){
	    *(board+i) = malloc(sizeof(int) * size);
	    if (*(board+i) == NULL){
		exit(1);
	    }
    }
    
	
	char *line = NULL;
	size_t len = 0;
	char *token = NULL;
	for (int i = 0; i < size; i++) {

		if (getline(&line, &len, fp) == -1) {
			printf("invalid\n");
			exit(1);
		}

		token = strtok(line, DELIM_DELIMITER);
		for (int j = 0; j < size; j++) {
			
			*(*(board+i)+j) = atoi(token);
			token = strtok(NULL, DELIM_DELIMITER);
		}
	}


    int if_valid = valid_board(board, size);
    if (if_valid == 1){
	    printf("valid\n");
    } else {
	    printf("invalid\n");
    }
    
	
    for (int i = 0; i < size; i++){
            free(*(board+i));
    }
    free(board);
    board = NULL;
    
		if (fclose(fp) != 0) {
		printf("invalid\n");
		exit(1);
	} 

	return 0;       
}       

