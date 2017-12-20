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
 * This handles all functionalities related to the memory chunks and the data
 * structure they are stored in.
 *
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


/*Initializes the starting memory available*/
mlist_t* initStartMem(int total_mem){
	segment_t *new;
	mlist_t *temp = (struct mlist*) malloc(sizeof(struct mlist));
	
	new = initSeg(total_mem, total_mem);
	temp->head = new;
	return temp;
}


/*Initializes a chunk of free memory with index*/
segment_t* initSeg(int size, int index) {
	segment_t* temp = (struct segment*) malloc(sizeof(struct segment));
	temp->size = size;
	temp->index = index;
	temp->next = NULL;
	return temp;
}

/*Adds a memory chunk of size at the index specified*/
segment_t* insertAtIndex(segment_t* head, int size, int index) {
	if(head->index == index)
	{
		head->index -= size;
		head->size -= size;
		if(head->size == 0){
			return head->next;
		}
		return head;
	}

	head->next = insertAtIndex(head->next, size, index);
	return head;
}

/*Frees a section of memory occupied by a process*/
segment_t* freeMemory(segment_t* head, segment_t* process) {
	if(head == NULL) {
		return process;
	}

	else if(process->index < head->index)
	{
		head->next = freeMemory(head->next, process);
		return head;
	}

	process->next = head;
	return process;
}

/* Checks if has space in memory, returns 1 if there is a space that can 
 * accomodate the size and 0 otherwise
 */
int hasSpace(mlist_t *flist ,int size) {
	segment_t *current;
	if(flist->head ==  NULL){
		return 0;
	}
	if(flist->head->size == size){
		return 1;
	}
	current = flist->head;
	while(current){
		if(current->size > size){
			return 1;
		}
		current = current->next;
	}
	return 0;
}

/* Gets a hole that a process can fit into based on the algorithm specified
 * returns -1 if no hole found;
 */
int getFit(char *algo, segment_t *head, int size){
	segment_t *current;
	int biggest, smallest, index;
	current = head;
	biggest = 0;
	smallest = 0;
	index = -1;
	
	/*Picks algo based on argument*/
	if(strcmp(algo, "first") == 0){
		while(current != NULL){
			if(current->size >= size){
				if(index == -1){
					return current->index;
				}
			}
			current = current->next;
		}
	}
	else if(strcmp(algo, "best") == 0){
		while(current != NULL){
			/*printf("best found\n");*/
			/*Gets smallest fitting hole*/
			if(current->size >= size){
				if(index == -1){
					index = current->index;
					smallest = current->size;
				}
				else if(current->size < smallest){
					index = current->index;
					smallest = current->size;
				}
			}
			current = current->next;
		}
	}
	else if(strcmp(algo, "worst") == 0){
		while(current != NULL){
			/*printf("Worst found\n");*/
			/*Gets biggest fitting hole*/
			if(current->size >= size){
				if(index == -1){
					index = current->index;
					biggest = current->size;
				}
				else if(current->size > biggest){
					index = current->index;
					biggest = current->size;
				}
			}
			current = current->next;
		}
	}
	return index;
}

/*Returns the number of holes*/
int countHoles(mlist_t *flist){
	int count;
	segment_t *current;
	
	count = 0;
	current = flist->head;
	while(current){
		count++;
		current = current->next;
	}
	return count;
}

/*Gets the total number of free space available*/
int totalSpace(segment_t *head){
	int count;
	segment_t *current;
	
	count = 0;
	current = head;
	while(current){
		count += current->size;
		current = current->next;
	}
	return count;
}


/*Loops through the free list looking for adjacent spaces to merge*/
void combineAdjacent(segment_t* head){
	if(head->next == NULL) {
		return;
	}
	else if((head->index - head->size) == (head->next->index))
	{
		head->size += head->next->size;
		head->next = head->next->next;
		combineAdjacent(head);
		return;
	}

	combineAdjacent(head->next);
	return;
}

/*Combines adjacent holes in the free memory list
void combineAdjacent(mlist_t *flist){
	mlist_t *current;
	current = flist;
	if(flist == NULL){
		return;
	}
	while(current->next){
		if(current->segment->index - current->segment->size 
		== current->next->segment->index){
			current->segment->size += current->next->segment->size;
			current->next = current->next->next;
		}
		else{
			current = current->next;
		}
	}
}*/