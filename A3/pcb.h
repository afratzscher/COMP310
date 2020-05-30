#ifndef PCB_H
#define PCB_H

typedef struct{
	int PC;
	int pid;
	int pageTable[10];
	int PC_page;
	int PC_offset;
	int pages_max; 
} PCB;

PCB* makePCB(int pid, int pages_max);

#endif
