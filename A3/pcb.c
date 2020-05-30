#include <stdio.h>
#include <stdlib.h>
#include "ram.h"

struct PCB {
	int PC;
	int pid;
	int pageTable[10];
	int PC_page;
	int PC_offset;
	int pages_max; 
};

struct PCB* makePCB(int pid, int pages_max){
	struct PCB* new = (struct PCB*)malloc(sizeof(struct PCB));
	new->PC = 0;
	new->pid = pid;
	new->PC_page = 0;
	new->PC_offset = 0;
	new->pages_max = pages_max;

	// set frame to -1 if not in physical mem
	for (int i = 0; i < 10; i++){
		new->pageTable[i] = -1;
	}

	return new;
}
