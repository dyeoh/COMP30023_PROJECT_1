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
 * This handles all functionalities related to the process and the data
 * structure it is stored in
 *
 *
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "processlist.h"


/*Initializes a new process*/
process_t* initProc(int create_time, int pid, int size, int jobtime){
	process_t *temp = (struct process*) malloc(sizeof(struct process));
	temp->timeCreated = create_time;
	temp->pid = pid;
	temp->size = size;
	temp->jobtime = jobtime;
	temp->disktime = 0;
	temp->memtime = 0;
	temp->runtime = 0;
	return temp;
}

/*Initializes a list of processes*/
plist_t *initList()
{
	plist_t *temp = (struct plist*) malloc(sizeof(struct plist));
	temp->head = NULL;
	return temp;
}

/*Stores queued processes to be created at an assigned time*/
plist_t* storeProcess(FILE* fr)
{
	int create_time, pid, size, jobtime;
	plist_t* temp = initList();


	while(fscanf(fr, "%d %d %d %d", &create_time, &pid, &size, &jobtime) == 4)
	{
		process_t *proc = initProc(create_time, pid, size, jobtime);
		temp->head = insertList(temp->head, proc);
	}
	fclose(fr);

	return temp;
}

/*Inserts a new process at the end of the list*/
process_t* insertList(process_t *head, process_t *process){
	if(head==NULL){
		return process;
	}
	else{
		head->next = insertList(head->next, process);
		return head;
	}
}


/*Pops the first item off the list and returns it*/
process_t* popList(plist_t *list) {
	process_t *temp = list->head;
	list->head = temp->next;
	temp->next = NULL;
	return temp;
}


/*Inserts a process into the disk*/
process_t* insertDisk(process_t *head, process_t *process){
	if(head==NULL) {
		return process;
	}
	else if((process->disktime == head->disktime) 
		&& (process->pid < head->pid))
	{
		process->next = head;
		return process;
	}
	else
	{
		head->next = insertDisk(head->next, process);
		return head;
	}
}

/*Inserts process onto disk
void insertDisk(process_t *head, process_t *process){
	process_t *current;
	
	if(head == NULL){
		printf("1\n");
		head = process;
	}
	else if((process->disktime == head->disktime) 
		&& process->pid < head->pid){
		printf("2\n");
		process->next = head;
		head = process;
	}
	else{
		current = head->next;
		while(current->next){
			current=current->next;
		}
		current->next = process;
	}
}*/


/*Removes process from anywhere in list*/
process_t* removeProcess(plist_t *plist, int pid){
	process_t *current, *prev;
	process_t *temp;
	temp = NULL;
	if (plist->head->pid == pid) {
        temp = plist->head;
		plist->head = temp->next;
    }
	else {
		current = plist->head->next;
		prev = plist->head;
		while(current != NULL && prev != NULL){
			if(current->pid == pid){
				temp = current;
				prev->next = current->next;
			}
			prev = current;
			current = current->next;
		}
	}
	return temp;
}


/*Returns the pid of the oldest process in the list*/
int findOldest(process_t *head){
	int oldest, pid;
	process_t *current;
	oldest = -1;
	pid = -1;
	current = head;
	while(current){
		if(current->memtime > oldest 
		&& current->pid >=0){
			oldest = current->memtime;
			pid = current->pid;
		}
		else if(current->memtime > oldest 
		&& current->pid >=0 
		&& current->pid < pid){
			oldest = current->memtime;
			pid = current->pid;
		}
		current=current->next;
	}
	return pid;
}

/*Returns number of elements in the list*/
int listLength(process_t *head){
	process_t *current;
	int count = 0;
	current = head;
	
	while(current){
		count ++;
		current = current->next;
	}
	return count;
}

