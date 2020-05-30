#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "ready_queue.h"
#include "cpu.h"
#include "memorymanager.h"

int quantadef = 2;
int flag;
FILE *fp;
int pid_int;

void addToReady(PCB* newPCB){
	struct ReadyQueueNode* temp = (struct ReadyQueueNode*)malloc(sizeof(struct ReadyQueueNode));
	temp->pcb = newPCB;
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

int kernel(){
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
	//make PCB for program
	pid_int++;
 	PCB* new = makePCB(pid_int, (ram_size/4)-1);
	addToReady(new);

	launcher(fp);
	return 0;
}

int scheduler(){
	// assumes quanta=0 means quanta finished, quanta=2 means can run 2 lines
	while (head != NULL){
		if (cpu->quanta == 0 || strcmp(cpu->IR, "") == 0){
			cpu->quanta = 2;
			cpu->IP = head->pcb->PC + head->pcb->PC_offset;
			run(cpu->quanta);	
		}
	}

	head = NULL;
	tail = NULL;

	return 0;
}

int boot(){
	int error = 0;
	// Part 1: initializes every cell of ram to NULL
	for (int i = 0; i < ram_size; i++){
		ram[i] = NULL;
	}

	// Part 2
	// clears backing store
	error = system("exec rm -rf ./BackingStore/*");
	if (error != 0){
		printf("Error clearing backingstore");
		return error;
	}
	// deletes old backing store
	system("exec rmdir ./BackingStore");
	if (error != 0){
		printf("Error deleting old backingstore");
		return error;
	}
	// makes new backingstore directory
	system("exec mkdir ./BackingStore");
	pid_int = 0;

	return error;

}

int main(){
	int error = 0;
	boot();
	error = kernel();
	return error;
}
