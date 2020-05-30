#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "kernel.h"
#include "ram.h"
#include "pcb.h"
#include "ready_queue.h"

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);

int countTotalPages(FILE *f){
	int count = 0;
	char c;
	for (c = getc(f); c != EOF; c = getc(f)){
		if (c == '\n'){
			count = count+1;
		}
	}
	int pages = (count+(4-1))/4;
	return pages;
}

void loadPage(int pageNumber, FILE *f, int frameNumber){
	int startLine = (pageNumber * 4);
	int endLine = startLine + 4;
	int startRam = (frameNumber * 4); //assumes first frame = 0

	rewind(f); // go back to start of file

	char line[1000];
	int count = 0;
	fgets(line,999,f);
	while (!feof(f)){
		if (count >= startLine && count < endLine){
			ram[startRam] = strdup(line);
			startRam++;
		}
		fgets(line, 999, f);
		count++;
	}
}

int findFrame(){
	int frameno = 0;
	int no = -1;
	for (int i = 0; i < 40; i+=4){
		if (ram[i] == NULL){
			no = i;
			break;
		}
	}
	if (no == -1){ return -1;}
	frameno = (no+(4-1))/4;
	return frameno;
}

int findVictim(PCB *p){
	srand(time(0));
	int victim;
	int randomNum = rand()%p->pages_max;
	int found = 0;
	for (int i = 0; i < p->pages_max; i++){
		if (p->pageTable[i] == randomNum){
			found = 1;
			break;
		}
	}
	if (found == 0){
		victim = randomNum;
	}
	else{
		for (int counter = 0; counter < p->pages_max; counter++){
			found = 0;
			int j = (randomNum+counter)%(p->pages_max);
			for (int l = 0; l < p->pages_max; l++){
				if (p->pageTable[l] == j){
					found = 1;
					break;				
				}
			}
			if (found == 0){
				victim = j;
				break;
			}
		}
	}
	return victim;
}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame){
	if (victimFrame == -1){ // means from loadPage
		p->pageTable[pageNumber] = frameNumber;
	}
	else { // means from findVictim
		p->pageTable[pageNumber] = victimFrame;
	}
	return 0;
}	

void callLoad(int numPages, int framestart, FILE *fp, PCB *p, int victim, int pageNumber) {
	if (numPages == 0){
		printf("No pages to load");
	} else if (numPages == 1){
		loadPage(pageNumber, fp, framestart);
		updatePageTable(p, pageNumber,framestart, victim);
	} else {
		loadPage(pageNumber, fp, framestart);
		updatePageTable(p, pageNumber, framestart, victim);
		loadPage(pageNumber+1, fp, framestart+1);
		updatePageTable(p, pageNumber+1, framestart+1, victim);
	}
}

int launcher(FILE *source){
	int error = 0;
	
	// 1: copy file
	FILE *target;
	char ch;
	char pid[3];
	sprintf(pid, "%d", pid_int); // make pid into string -> max pid = 999

	char txt[] = ".txt";
	strcat(pid, txt);
	char loc[] = "./BackingStore/";
	char input[100];
	snprintf(input, 100, "exec touch %s%s", loc,pid); // exec touch ./BackingStore/pid.txt
	system(input); // make copy in BackingStore

	strcat(loc, pid);
	target = fopen(loc, "wb");
	if (target == NULL){
		return -1;
	}
	while ((ch = fgetc(source)) != EOF){
		fputc(ch, target);
	}
	fclose(target);
	
	// 2: close original file
	fclose(source);

	// 3: open file in backingstore
	FILE *fp;
	fp = fopen(loc, "rbt");
	if (fp == NULL){
		printf("Error opening %s%s", loc, pid);
		return -1;
	}
	
	//4: loading pages
	int numPages = countTotalPages(fp);
	int framestart = findFrame();
	
	// find PCB for process
	PCB *p;
	struct ReadyQueueNode* current = head;
	while(current){
		if (current->pcb->pid == pid_int){
			p = current->pcb;
			break;
		}
		else{
			current = current->next;
		}
	}

/*	//to remove
	printf("BEFORE\n");
	for (int i = 0; i < 10; i++){
		printf("%d page: %d frame\n", i, p->pageTable[i]);
	}
	*/

	if (framestart != -1){ // if frame available, add
		callLoad(numPages, framestart, fp, p, -1, 0); // -1  meaning not victim
	}
	else{ // if framestart = -1, means ram full -> need to find victim
		int victim = findVictim(head->pcb);
		callLoad(numPages, victim, fp, p, victim, 0);	
	}

/*	//to remove
	for(int i =0; i <40;i++){
		printf("%s", ram[i]);
	}

	// to remove
	printf("\n");
	for (int i = 0; i < 10; i++){
		printf("%d page: %d frame\n", i, p->pageTable[i]);
	}
*/
	fclose(fp);
	return error;
}

