///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission SPRING 2023, CS354-deppeler
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        p3Heap.c
// This File:        p3Heap.c
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

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"
 
/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

    int size_status;

    /*
     * Size of the block is always a multiple of 8.
     * Size is stored in all block headers and in free block footers.
     *
     * Status is stored only in headers using the two least significant bits.
     *   Bit0 => least significant bit, last bit
     *   Bit0 == 0 => free block
     *   Bit0 == 1 => allocated block
     *
     *   Bit1 => second last bit 
     *   Bit1 == 0 => previous block is free
     *   Bit1 == 1 => previous block is allocated
     * 
     * Start Heap: 
     *  The blockHeader for the first block of the heap is after skip 4 bytes.
     *  This ensures alignment requirements can be met.
     * 
     * End Mark: 
     *  The end of the available memory is indicated using a size_status of 1.
     * 
     * Examples:
     * 
     * 1. Allocated block of size 24 bytes:
     *    Allocated Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 25
     *      If the previous block is allocated p-bit=1 size_status would be 27
     * 
     * 2. Free block of size 24 bytes:
     *    Free Block Header:
     *      If the previous block is free      p-bit=0 size_status would be 24
     *      If the previous block is allocated p-bit=1 size_status would be 26
     *    Free Block Footer:
     *      size_status should be 24
     */
} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables needed by your function
 */


/*
 * Helper function
 * returns size - p and a bits 
 */ 
int getSize(blockHeader* bh) {
	int a = 0; 
	int p = 0;
	int size = bh->size_status;
	if (size % 8 == 3) {
		a = 1;
		p = 2;
	} else if (size % 8 == 2) {
		a = 0;
		p = 2;
	} else if (size % 8 == 1) {
		a = 1;
		p = 0;
	} 
	size = size - (a + p);
	return size;
}

/*
 * Helper function
 * returns 1 if free 0 if not 
 */ 
int isFree(blockHeader* bh) {
	if ((bh->size_status & 1) == 0) {
		return 1;
	} else {
		return 0;
	}
}



 
/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return if NULL unable to find and allocate block for required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) {     
    
    //TODO: Your code goes in here.
    if (size <= 0) return NULL;
	
	int blockSize = 4 + size;
	if (blockSize % 8 != 0) blockSize = blockSize + (8 - (blockSize % 8));

	blockHeader* currentHeader = heap_start;

	blockHeader* best = NULL;
	int currSize;
	int perfect = 0;

	while (currentHeader -> size_status != 1){
		currSize = getSize(currentHeader);
		if (isFree(currentHeader)) {
			if (blockSize == currSize){
				best = currentHeader;
				perfect = 1;
				break;
			}

			else if (currSize > blockSize) {
				if (best == NULL) {
					best = currentHeader;
				}
				else if ((best -> size_status) > (currentHeader -> size_status)) {
					best = currentHeader;
				}
			}	
		}   
		currentHeader = (blockHeader*) ((void*) currentHeader + currSize);
	}
	if (best == NULL) return NULL;

	int bestSize = getSize(best);
	blockHeader *next = (blockHeader*) ((void*) best + bestSize);
	if (perfect) {
		if (next -> size_status == 1) {
			best -> size_status += 1;
			return ((void*)best + sizeof(blockHeader));
		}
		else {
			next->size_status += 2;

			best-> size_status += 1;
			return ((void*)best + sizeof(blockHeader));
		}
	}
	else {
		if ((bestSize - blockSize) % 8 == 0) {
			blockHeader *split = (blockHeader*) ((void*) best + blockSize);
			split -> size_status = (bestSize - blockSize) + 2;

			blockHeader *footer = (blockHeader*) ((void*)split
					+ (bestSize - blockSize) - sizeof(blockHeader));
			footer -> size_status = bestSize - blockSize;
			if((best -> size_status) % 8 == 2){
				best -> size_status = blockSize + 1 + 2;
			}
			else {
				best -> size_status = blockSize + 1;
			}
		}

		else best -> size_status += 1;
		return ((void*) best + sizeof(blockHeader));
	}
	return NULL;
} 
 
/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int bfree(void *ptr) {    
    
    //TODO: Your code goes in here.
	if (ptr == NULL) return -1;

	if ((unsigned int)ptr % 8 != 0) return -1;

	blockHeader* ptrHeader = (blockHeader*) ((void*) ptr - sizeof(blockHeader));
	int ptrSize = getSize(ptrHeader);

	if (isFree(ptrHeader)) return -1;

	blockHeader* parse = heap_start;
	int size = 0;

	while (parse->size_status != 1) {
		size = getSize(parse);
		parse = (blockHeader*) ((void*) parse + size);
	}
	if (ptr > (void*)parse) return -1;

	ptrHeader->size_status -= 1;

	blockHeader* next = (blockHeader*) ((void*) ptrHeader + ptrSize);
	if(next->size_status != 1) next->size_status -= 2;
	return 0;
	
} 



/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for user-called coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
    
    //TODO: Your code goes in here.
	int count = 0;
	blockHeader* currentHeader = heap_start;
	int currSize = getSize(currentHeader);
	blockHeader* next = (blockHeader*) ((void*) currentHeader + currSize);
	int nextSize; 
	while (currentHeader->size_status != 1) {
		currSize = getSize(currentHeader);
		if (isFree(currentHeader)) {
			next = (blockHeader*) ((void*) currentHeader + currSize);
			nextSize = getSize(next);
			while (isFree(next)) {

				currentHeader-> size_status += nextSize;
				currSize = getSize(currentHeader);
				count++;
				next = (blockHeader*) ((void*) currentHeader + currSize);
			}
		}
		currentHeader = (blockHeader*) ((void*) currentHeader + currSize);
	}
	return count;
}

 
/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    
 
    static int allocated_once = 0; //prevent multiple myInit calls
 
    int   pagesize; // page size
    int   padsize;  // size of padding when heap size not a multiple of page size
    void* mmap_ptr; // pointer to memory mapped area
    int   fd;

    blockHeader* end_mark;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: InitHeap has allocated space during a previous call\n");
        return -1;
    }

    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize from O.S. 
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (MAP_FAILED == mmap_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
    allocated_once = 1;

    // for double word alignment and end mark
    alloc_size -= 8;

    // Initially there is only one big free block in the heap.
    // Skip first 4 bytes for double word alignment requirement.
    heap_start = (blockHeader*) mmap_ptr + 1;

    // Set the end mark
    end_mark = (blockHeader*)((void*)heap_start + alloc_size);
    end_mark->size_status = 1;

    // Set size in header
    heap_start->size_status = alloc_size;

    // Set p-bit as allocated in header
    // note a-bit left at 0 for free
    heap_start->size_status += 2;

    // Set the footer
    blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
} 
                  
/* 
 * Function can be used for DEBUGGING to help you visualize your heap structure.
 * Traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     
 
    int    counter;
    char   status[6];
    char   p_status[6];
    char * t_begin = NULL;
    char * t_end   = NULL;
    int    t_size;

    blockHeader *current = heap_start;
    counter = 1;

    int used_size =  0;
    int free_size =  0;
    int is_used   = -1;

    fprintf(stdout, 
	"*********************************** HEAP: Block List ****************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => used block
            strcpy(status, "alloc");
            is_used = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "FREE ");
            is_used = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "alloc");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "FREE ");
        }

        if (is_used) 
            used_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blockHeader*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, 
	"---------------------------------------------------------------------------------\n");
    fprintf(stdout, 
	"*********************************************************************************\n");
    fprintf(stdout, "Total used size = %4d\n", used_size);
    fprintf(stdout, "Total free size = %4d\n", free_size);
    fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
    fprintf(stdout, 
	"*********************************************************************************\n");
    fflush(stdout);

    return;  
} 


// end p3Heap.c (Spring 2023)                                         


