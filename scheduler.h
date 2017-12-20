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

/*Handles updating the utilities as time passes*/
void incrementTime(plist_t *disk, plist_t *robin);

/*Swaps processes in between disk and memory based on the algorithm chosen*/
void swap(plist_t *robin, plist_t *disc, mlist_t *free_list, char *algo);

//Checks and terminates program once its job time has been reached
void terminate(plist_t *robin, mlist_t *free_list);

/*Handles the scheduling process*/
void schedule(plist_t *robin, mlist_t *free_list);

/*Handles the events, returns 1 if an event occurs and 0 otherwise */
int eventHandler(plist_t *disk, plist_t *robin, int quantum);

/*Handles loading processes to the disk at their respective creation times*/
void loadToDisk(plist_t *waitQ, plist_t *disc, int time);