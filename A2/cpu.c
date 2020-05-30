#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ram.h"
#include "interpreter.h"
#include "ready_queue.h"
#include "shell.h"
#include "kernel.h"

struct CPU {
	int IP;	
	char IR[1000]; 
	int quanta;
};

struct CPU* cpu;

void run(int quanta){
	int givenQuanta = quanta;
	//copy quanta lines of code into IR OR until script file is at end
	int current = head->pcb.PC;
	while (quanta > 0 && current <= head->pcb.end){
		strcpy(cpu->IR, ram[current]);
		quanta--;
		current = current+1;
	//NOTE: should be interpreter not parse BUT interpreter takes char []words
		parse(cpu->IR);
	}
	
	//update PCB PC when quanta done
	head->pcb.PC = head->pcb.PC+givenQuanta;

	//if program not done, add to end of ready queue
	if (head->pcb.PC <= head->pcb.end){
		struct ReadyQueueNode* current = head;
		
		// if more than one program
		if (head->next != NULL){
			head = current->next;
			tail->next = current;
			current->next = NULL;
			tail = current;
		}
		//if one program
		else{
			head = current;
		}
	}
	
	//if program done, PCB terminates
	else{
		clearFromRAM(head->pcb.start, head->pcb.end);

		// remove from ready queue
		struct ReadyQueueNode* current = head;
		head = current->next;
		free(current);
	}

	cpu->quanta = 0;
}

