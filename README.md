# COMP30023_PROJECT_1
A memory scheduling simulator using a round robin algorithm. The simulator aims to simulate a CPU, taking processes
of different sizes, loads them into memory when required, using one of three different algorithms (first fit, best fit, worst fit) 
to swap out processes to create a sufficiently large space for the process to be loaded into memory. The processes that are currently in memory are handled using the round robin algorithm.

## Getting Started
A precompiled executable named swap is provided in the local directory of the project. If you
wish to compile the executable yourself, running the included makefile will result
in an executable named swap being created in the root directory of the project. 

### Guide
The program takes in four arguments which are needed for the program to run. The list of options are listed below:

1.  Selecting an algorithm
```
-a <first/best/worst>
```
  Used to select the algorithm used by the simulation.

#### First Fit 
```
-a first
```

First ﬁt: First ﬁt starts searching the free list from the beginning (highest address), and uses the ﬁrst hole large enough to satisfy the request. If the hole is larger than necessary, it is split, with the process occupying the higher address range portion of the hole and the remainder being put on the free list. 

#### Best Fit 
```
-a best
```
Best ﬁt: Chooses the smallest hole from the free list that will satisfy the request. If multiple holes meet this criterion, choose the highest address one in the free list. If the hole is larger than necessary, it is split, with the process occupying the higher address range portion of the hole and the remainder being put on the free list. 

#### Worst Fit
-a worst

Worst ﬁt: Chooses the largest hole from the free list that will satisfy the request. If multiple holes meet this criterion, choose the highest address one in the free list. If the hole is larger than necessary, it is split, with the process occupying the higher address range portion of the hole and the remainder being put on the free list.

2. Reading Processes from a file
```
-f <filename>
```
  Takes in the file with the sequence of processes and their data. 
  Each process in the line sequence follow this format : time-created, process-id, memory-size, job-time

File content example:
```
0 3 85 30 
5 1 100 20 
20 6 225 15 
24 10 50 14

Where process 3 is created at time 0, is 85 MB in size, and needs 30 seconds running time to ﬁnish.
```

3. Total memory of CPU
```
-m <total_mem>
```
Used to set the total main memory allocated to the scheduler.

4. Setting the quantum of a process used in Round Robin
```
-q <quantum_time>
```
Used to set the quantum which is the time in seconds a process can run before it is swapped out using the round robin algorithm. 

### Example Usage
A few sequences of processes have been provided to assist you with testing the program (testBest.txt, testWorst.txt, testFirst.txt).

The example below shows the output using the best fit algorithm on the processes in testBest.txt with 1000MB of main memory and a quantum of 2.

```
./swap -a best -f testBest1.txt -m 1000 -q 2
time 0, 4 loaded, numprocesses=1, numholes=1, memusage=4%
time 2, 6 loaded, numprocesses=2, numholes=1, memusage=14%
time 6, 3 loaded, numprocesses=3, numholes=1, memusage=24%
time 8, 5 loaded, numprocesses=4, numholes=1, memusage=24%
time 10, 9 loaded, numprocesses=5, numholes=1, memusage=74%
time 12, 12 loaded, numprocesses=6, numholes=1, memusage=79%
time 16, 14 loaded, numprocesses=4, numholes=2, memusage=77%
time 18, 3 loaded, numprocesses=5, numholes=2, memusage=87%
time 20, 4 loaded, numprocesses=6, numholes=2, memusage=91%
time 22, 6 loaded, numprocesses=5, numholes=2, memusage=50%
time 24, 7 loaded, numprocesses=6, numholes=2, memusage=62%
time 26, 15 loaded, numprocesses=4, numholes=3, memusage=65%
time 28, 24 loaded, numprocesses=5, numholes=3, memusage=75%
time 30, 5 loaded, numprocesses=6, numholes=3, memusage=75%
time 32, 9 loaded, numprocesses=3, numholes=2, memusage=61%
time 34, 3 loaded, numprocesses=4, numholes=2, memusage=71%
```
