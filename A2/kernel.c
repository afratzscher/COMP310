#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "ready_queue.h"
#include "cpu.h"

int quantadef = 2;
int flag;
FILE *fp;

void addToReady(PCB* newPCB){
	struct ReadyQueueNode* temp = (struct ReadyQueueNode*)malloc(sizeof(struct ReadyQueueNode));
	temp->pcb = *newPCB;
	if (head == NULL){
		temp->next = NULL;
		head = temp;
		tail = temp;
	}
	else{ // add new node to END of queue (because FIFO)
		struct ReadyQueueNode* current = tail;
		current->next = temp;
		tail = temp;
	}
}

int main(){
	head = NULL;
	tail = NULL;
	cpu = (struct CPU*)malloc(sizeof(struct CPU));
	cpu->IP = 0;
	strcpy(cpu->IR, "");
	cpu->quanta = 0;
	shellUI();
}

int myinit(char *filename){
	fp = fopen(filename, "rt");
	if (fp == NULL){ // failed to open file
		return -1;
	}
	int start;
	int end;
	addToRAM(fp, &start, &end);
	fclose(fp);
	
	if (end == -1){
		return -1;
	}

 	PCB* new = makePCB(start, end); 
	addToReady(new);
	return 0;
}

int scheduler(){
	// assumes quanta=0 means quanta finished, quanta=2 means can run 2 lines
	while (head != NULL){
		if (cpu->quanta == 0 || strcmp(cpu->IR, "") == 0){
			cpu->quanta = 2;
			cpu->IP = head->pcb.PC;
			run(cpu->quanta);	
		}
	}
	
	head = NULL;
	tail = NULL;

	return 0;
}
