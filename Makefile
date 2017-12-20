# 
# Project 1 Makefile
# username: dyeoh
# student number: 715863
#

## CC  = Compiler.
## CFLAGS = Compiler flags.
CC	= gcc
CFLAGS =-Wall -Wextra -std=gnu99


## OBJ = Object files.
## SRC = Source files.
## EXE = Executable name.

SRC =		scheduler.c processlist.c memory.c 
OBJ =		scheduler.o processlist.o memory.o 
EXE = 		swap

## Top level target is executable.
$(EXE):	$(OBJ)
		$(CC) $(CFLAGS) -o $(EXE) $(OBJ)


## Clean: Remove object files and core dump files.
clean:
		/bin/rm $(OBJ) 

## Clobber: Performs Clean and removes executable file.

clobber: clean
		/bin/rm $(EXE) 

## Dependencies

scheduler.o: scheduler.h processlist.h memory.h
memory.o: memory.h
processlist.o: processlist.h
