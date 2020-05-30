#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ram.h"
#include "interpreter.h"
#include "ready_queue.h"
#include "shell.h"
#include "kernel.h"
#include "memorymanager.h"

struct CPU {
	int IP;	
	int offset;
	char IR[1000]; 
	int quanta;
};

struct CPU* cpu;

void addToEnd(){
	struct ReadyQueueNode* current = head;
	// more than one program
	if (head ->next != NULL){
		head = current->next;
		tail->next = current;
		current->next = NULL;
		tail = current;
	}
	// one program
	else{
		head = current;
	}
}

void removePCB(){
	struct ReadyQueueNode* curr = head;
	head = curr->next;
	free(curr);
}

void deleteBSFile(){
	char pid[3];
	sprintf(pid, "%d", head->pcb->pid);
	char txt[] = ".txt";
	strcat(pid, txt);
	char loc[] = "./BackingStore/";
	char input[100];
	snprintf(input, 100, "exec rm %s%s", loc, pid);
	system(input);
}

void run(int quanta){
	int givenQuanta = quanta;

	//get page for process
	PCB *p = head->pcb;
	int frame;
	for (int i = 0; i < p->pages_max; i++){
		if (p->pageTable[i] != -1){
			frame = p->pageTable[i];
			break;
		}
	}
	
	// run until offset > 3 OR use up quanta
	while(givenQuanta > 0 && p->PC_offset < 4){
		p->PC = 4*(frame) + p->PC_offset;
		if (ram[p->PC] != NULL){
			strcpy(cpu->IR, ram[p->PC]);
			givenQuanta--;
			parse(cpu->IR);
			ram[p->PC] = NULL;
		}
		p->PC_offset++;
	}

	// if quanta executed
	if (givenQuanta == 0){
	//	printf("add to end");
		int index = p->pageTable[p->PC_page];
		if (ram[index] == NULL && ram[index+1] == NULL
				&& ram[index+2] == NULL 
				&& ram[index+3] == NULL){
			// if all lines executed, set frame to -1
			p->pageTable[p->PC_page] = -1;
		//	printf("set %d", p->PC_page);
		} 
		addToEnd();
	}

	// else (ie. found at least one page not executed) = page fault
	else {
	//	printf("page fault");
		//1) increment PC_page
		p->PC_page++;
		//printf("PC page %d", p->PC_page);
		
		//2) if > pages_max, terminate (dont do anything because already cleared ram during execution); else check if frame exists in pageTable
	//	printf("%d max", p->pages_max);
		if (p->PC_page > p->pages_max){
			 // dont clear ram because have been setting ram = NULL while executing
			 // need to remove PCB and remove BS file
			 deleteBSFile();
			 removePCB();
		}
		else{

			//3) if valid
		//	printf("val %d", p->pageTable[p->PC_page]);
			if(p->pageTable[p->PC_page] != -1){
			//	printf("FOUND");
				p->PC = 4*(p->pageTable[p->PC_page]);
				p->PC_offset = 0;
				addToEnd();
			}
			// 4) if not valid
			else {
				PCB *p = head->pcb;
				char pid[3];
				sprintf(pid, "%d", p->pid);
				char txt[] = ".txt";
				strcat(pid, txt);
				char loc[] = "./BackingStore/";
				char input[100];
				snprintf(input, 100, "%s%s", loc,pid);
				
				FILE *fp;
				fp = fopen(input, "rbt");
				if (fp == NULL){
					printf("Error opening file");
				}

				int pagesTotal = countTotalPages(fp);
				
				// if all pages read
				if (p->PC_page > pagesTotal){
					deleteBSFile();
					removePCB();
					fclose(fp);
				}
				else{
					int frame = findFrame();
					if (frame == -1){ // need victim
					//	printf("VICTIM");
						int victim = findVictim(p);
						callLoad(1, victim, fp, p, victim, p->PC_page);
					}
					else{
						callLoad(1, frame, fp, p, -1, p->PC_page);
					}
					fclose(fp);
					p->PC = 4*(p->pageTable[p->PC_page]);
					p->PC_offset = 0;
					/*printf("UPDATED");
					for(int i =0;i<p->pages_max;i++){
						printf("%d pid\n", p->pid);
						printf("%d page: %d frame\n", i, p->pageTable[i]);}*/
					addToEnd();
				}

			}
		/*	printf("UPDATED");*/
		//	printf("%d pid", p->pid);
		/*	for (int i = 0; i < p->pages_max; i++){
				printf("%d page: %d frame\n", i, p->pageTable[i]);
			}
			printf("RAM");
			for (int i = 0; i <40; i++){
				printf("%s", ram[i]);}*/
		}	

	}

	cpu->quanta = 0;
	
}

