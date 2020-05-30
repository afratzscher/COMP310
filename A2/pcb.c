#include <stdio.h>
#include <stdlib.h>

struct PCB {
	int PC;
	int start;
	int end;
};

struct PCB* makePCB(int start, int end){
	struct PCB* new = (struct PCB*)malloc(sizeof(struct PCB));
	new->PC = start;
	new->start = start;
	new->end = end;
	return new;
}
