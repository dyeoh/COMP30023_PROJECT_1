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


/*The structure of a process*/
typedef struct process{
	int timeCreated;
	int pid;
	int size;
	int jobtime;
	int memtime;
	int disktime;
	int runtime;
	int index;
	struct process *next;
}process_t;

/*Handles a list of processes and the start point*/
typedef struct plist {
	process_t* head;
}plist_t;


/*Initializes a new process*/
process_t* initProc(int create_time, int pid, int size, int jobtime);

/*Initializes the list of processes*/
plist_t* initList();

/*Stores queued processes to be created at an assigned time*/
plist_t* storeProcess(FILE* fname);

/*Inserts a new process at the end of the list*/
process_t* insertList(process_t *head, process_t *process);

/*Inserts process into the back of the list
void insertAtEnd(process_t **head, process_t *process);*/

/*Pops the first item off the list and returns it*/
process_t* popList(plist_t *queue);

/*Inserts a process into the disk*/
process_t* insertDisk(process_t *head, process_t *process);

/*Removes process from anywhere in list*/
process_t* removeProcess(plist_t *plist, int pid);

/*Returns the pid of the oldest process in the list*/
int findOldest(process_t *head);

/*Returns number of elements in the list*/
int listLength(process_t *head);