///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission SPRING 2023, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
// Main File:        cache1D.c, cache2Drows.c, cache2Dcols.c and cache2Dclash.c
// This File:        cache2Drows.c
// Other Files:      cache1D.c, cache2Dclash.c, and cache2Dcols.c
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

#define ROWS 3000
#define COLUMNS 500

int arr2D[ROWS][COLUMNS];

int main(){

	int row;
	int col;


	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLUMNS; col++){
			arr2D[row][col] = row + col;
		}

	}

	return 0;
}