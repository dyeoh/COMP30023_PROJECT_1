/*
 * COMP30023 Computer Systems
 * Semester 1 2017
 * Project 1
 * Memory Management and Process Scheduling
 *
 * University Username: dyeoh
 * Name: Darren Yeoh Cheang Leng
 * Student ID: 715863
 *
 * This module handles all functionalities related to the memory chunks and the data
 * structure they are stored in.
 *
 *
*/



/*The struct for the connected memory segments*/
typedef struct segment{
	int index;
	int size;
	struct segment* next;
}segment_t;

/*The start of the connected memory segments*/
typedef struct mlist {
	segment_t* head;
}mlist_t;

/*Initializes the starting memory available*/
mlist_t* initStartMem(int total_mem);

/*Initializes a chunk of free memory starting at the highest index*/
segment_t* initSeg(int size, int index);

/*Adds a memory chunk of size at the index specified*/
segment_t* insertAtIndex(segment_t* head, int size, int index);

/*Frees a section of memory occupied by a process*/
segment_t* freeMemory(segment_t* head, segment_t* process);

/* Checks if has space in memory, returns 1 if there is a space that can 
 * accomodate the size and 0 otherwise
 */
int hasSpace(mlist_t *flist ,int size);

/* Gets a hole that a process can fit into based on the algorithm specified
 * returns -1 if no hole found;
 */
int getFit(char *algo, segment_t *head, int size);

/*Returns the number of holes*/
int countHoles(mlist_t *flist);

/*Gets the total number of free space available*/
int totalSpace(segment_t *head);

/*Loops through the free list looking for adjacent spaces to merge*/
void combineAdjacent(segment_t* root);