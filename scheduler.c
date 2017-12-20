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
 * This controls the entire program, including creating the processes, and 
 * handling the swapping and scheduling of the processes based on events or time
 * passed.
 *
 *
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "processlist.h"
#include "memory.h"
#include "scheduler.h"

#define MAX_LENGTH 256
#define INITIALIZED 1
#define UNINITIALIZED 0
#define FAILURE -1

int main(int argc, char** argv){
	/*Command line arguments*/
	int fileRead, quantum, total_mem;
	char algo[MAX_LENGTH] = "worst";
	char input;
	FILE *fr;
	
	/*Output data*/
	int freespace, memusage;
	
	/*System variables*/
	int time = 0;
	fileRead = 0;
	quantum = UNINITIALIZED;
	total_mem = UNINITIALIZED;
	plist_t *plist, *disk;
	
	
	/*Get command line arguments*/
	while ((input = getopt(argc, argv, "a:f:m:q:")) != EOF){
		switch ( input ){
			case 'a':
				if(strcmp(optarg, "first") == 0)
					strcpy(algo, optarg);

				else if(strcmp(optarg, "best") == 0) 
					strcpy(algo, optarg);

				else if(strcmp(optarg, "worst") != 0) 
				{
					fprintf(stderr, "Unknown algorithm entered : %s\n ", optarg);
					exit(FAILURE);
				}
 				break;
				
			case 'f':
				fr = fopen(optarg, "r");
				fileRead = INITIALIZED;
				break;
				
			case 'm':
				total_mem = atoi(optarg);
				break;
				
			case 'q':
				quantum = atoi(optarg);
				break;
				
			default:
				break;
		} 
	}
	
	/*Checks if values are initialized*/
	if(quantum == UNINITIALIZED){
		printf("Error: No quantum specified\n");
		exit(FAILURE);
	}
	if(total_mem == UNINITIALIZED){
		printf("Error: Main memory not set\n");
		exit(FAILURE);
	}
	if(fileRead == UNINITIALIZED){
		printf("Error: No file input\n");
		exit(FAILURE);
	}
	
	/*Initializes utilities*/
	plist = storeProcess(fr);
	disk = initList();
	plist_t *robin = initList();
	mlist_t *memory = initStartMem(total_mem);

	/*While there are still processes that need to be run*/
	while(!((plist->head==NULL) && (disk->head==NULL) && (robin->head==NULL)))
	{
		int flag = UNINITIALIZED;
		loadToDisk(plist, disk, time);

		/*If an event just passed*/
		if(eventHandler(robin,disk,quantum) == INITIALIZED){
			if(disk->head != NULL){
				printf("time %d, ", time);
				flag = INITIALIZED;
			}
			
			swap(robin, disk, memory, algo);
			schedule(robin, memory);

			freespace = totalSpace(memory->head);
			
			memusage = 100 - ((100*freespace) / total_mem);

            if(flag==INITIALIZED){
				printf("numprocesses=%d, ", listLength(robin->head));
			    printf("numholes=%d, ", countHoles(memory));
			    printf("memusage=%d%%\n", memusage);
			    flag = UNINITIALIZED;
			}
			
		}
		incrementTime(disk,robin);
		time++;
	}
	printf("time %d, simulation finished.\n", time-1);

	return 0;
}


/*Handles updating the utilities as time passes*/
void incrementTime(plist_t *disk, plist_t *robin) {
	process_t *current;
	if(robin->head != NULL){
		robin->head->runtime ++;
		robin->head->jobtime --;
	}
	current = disk->head;
	while(current){
		current->disktime ++;
		current = current->next;
	}
	current = robin->head;
	while(current){
		current->memtime ++;
		current = current->next;
	}
}

/*Handles the scheduling process*/
void swap(plist_t *robin, plist_t *disk, mlist_t *memory, char *algo){
	process_t *running, *swapout;

	if(disk->head == NULL){
		return;
	}
	
	terminate(robin, memory);
	running = popList(disk);
	running->disktime = UNINITIALIZED;

	printf("%d loaded, ", running->pid);

	while(hasSpace(memory, running->size) == 0){
		int old_pid = findOldest(robin->head);

		swapout = removeProcess(robin, old_pid);

		segment_t *free = initSeg(swapout->size, swapout->index);

		memory->head = freeMemory(memory->head, free);
		combineAdjacent(memory->head);

	    swapout->memtime = UNINITIALIZED;
		swapout->index = UNINITIALIZED;
		swapout->runtime = UNINITIALIZED;
		swapout->next = NULL;
		disk->head = insertDisk(disk->head, swapout);
	}
	
	/*Gets index of fir depending on algorithm*/
	int index = getFit(algo, memory->head, running->size);
	running->index = index;
	
	/*Add to rond robin queue and memory*/
	robin->head = insertList(robin->head, running);
	memory->head = insertAtIndex(memory->head, running->size, index);
}

/*Handles the scheduling process*/
void schedule(plist_t *robin, mlist_t *memory){
	if(robin->head == NULL){
		return;
	}
	if(robin->head->runtime == UNINITIALIZED) {
		return;
	}
	if(robin->head->jobtime == UNINITIALIZED){
		terminate(robin, memory);
		return;
	}

	/*Push back*/
	process_t *pop = popList(robin);
	pop->runtime = UNINITIALIZED;
	robin->head = insertList(robin->head, pop);
}

/*Checks if current running process has finished*/
void terminate(plist_t *robin, mlist_t *memory){
	if(robin->head==NULL){
		return;
	}
	if(robin->head->jobtime == UNINITIALIZED){
		process_t *terminate = popList(robin);
		segment_t *newfree = initSeg(terminate->size, terminate->index);
		memory->head = freeMemory(memory->head, newfree);
		combineAdjacent(memory->head);
	}
}


/*Handles the events, returns 1 if an event occurs and 0 otherwise */
int eventHandler(plist_t *robin, plist_t *disk, int quant){
	if(robin->head == NULL && disk->head != NULL) {
		return INITIALIZED;
	}
	else if(robin->head->runtime == quant) {
		return INITIALIZED;
	}
	else if(robin->head->jobtime == UNINITIALIZED) {
		return INITIALIZED;
	}
	return UNINITIALIZED;
}

/*Handles loading processes to the disk at their respective creation times*/
void loadToDisk(plist_t *plist, plist_t *disk, int time){
	if(plist->head == NULL) return;
	process_t *new;
	while(plist->head->timeCreated == time)
	{
		new = popList(plist);
		new-> disktime = UNINITIALIZED;
		disk->head = insertDisk(disk->head, new);
		if(plist->head == NULL) break;
	}
}
