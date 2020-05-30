#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "ram.h"
#include "kernel.h"
#include "ready_queue.h"

FILE *p; //file pointer for script to run
int fileOpen;

// help function prints list of commands and descriptions 
int help(){
	printf("COMMAND		 	   	 DESCRIPTION\n");
	printf("help			 	 Displays all commands\n");
	printf("quit				 Exits/terminates the shell with 'Bye!'\n");
	printf("set VAR STRING			 Assigns a value to shell memory\n");
	printf("print VAR			 Displays the STRING assigned to VAR\n");
	printf("run SCRIPT.TXT 			 Executes the file SCRIPT.TXT\n");
	printf("exec p1 p2 p3			 Executes concurrent programs\n");
	return 0;
}

// quit function: if in script, exit script and go back to shell; if in shell, exit shell
int quit(){
	//if no file open, exit shell
	printf("Bye!\n");
	if (fileOpen == 0){
		return -1;
	}

	// if one file open, exit script and return to shell
	return 0;
}

// set function sets variable name = value
int set(char *words[]) {
	if (strcmp(words[1], "") == 0 || strcmp(words[2], "") == 0){
		printf("Missing argument(s) for set command\n");
		return 0;
	}
	char *name = words[1];
	char *value = words[2];
	return setVar(name, value);
}

// print function prints value of variable name
int print(char *words[]) {
	if (strcmp(words[1], "") == 0){
		printf("Missing argument for print command\n");
		return 0;
	}
	char *name = words[1];
	return printVal(name);
}

// get lines for run function
int getlines(FILE *p){
	int errCode = 0;
	char line[1000];
	if ( p == NULL){
		printf("Script not found\n");
		return 0;
	}
	fileOpen++;
	fgets(line,999, p);
	while(!feof(p)){
		errCode = parse(line);
		fgets(line,999, p);
	}
	fclose(p);
	fileOpen--;
}

// run function opens file and executes commands line by line
// if file doesnt exist, return to shell
int static run(char *words[]) {
	int errCode = 0;
	p = fopen(words[1], "rt");
	errCode = getlines(p);
	return errCode;
}

//exec {
int exec(char *words[]){
	int errCode = 0;
	flag = 0;
	// need at least one argument
	if (strcmp(words[1], "") == 0){
		printf("Need at least one argument\n");
		return 0;
	}

	// check if arguments are .txt files
	char *text = ".txt";
	if (strstr(words[1], text) == NULL){
		words[1] = "";
	}
	if (strstr(words[2], text) == NULL){
		words[2] = "";
	}
	if (strstr(words[3], text) == NULL){
		words[3] = "";
	}
	 
	errCode = myinit(words[1]);

	// for ass3, CAN open same file multiple times
	if (strcmp(words[2], "") != 0){
		errCode += myinit(words[2]);
		/*if (strcmp(words[1], words[2]) == 0){
			printf("Error12: Script %s already loaded\n", words[1]);
		}
		else{
			errCode += myinit(words[2]);
		}*/
	}
	if (strcmp(words[3], "") != 0){
		errCode += myinit(words[3]);
		/*if (strcmp(words[1], words[3]) == 0){
			printf("Error13: Script %s already loaded\n", words[1]);
		} 
		else if (strcmp(words[2], words[3]) == 0){
			printf("Error23: Script %s already loaded\n", words[2]);
		}
		else {	
			errCode += myinit(words[3]);
		} */
	}	

	if (errCode != 0) {
		printf("Load error. Please input exec command again\n");
		//clear RAM and remove PCBs (else, if first argument loaded but not removed, if load again later, get duplicates)
		clearAllRAM();

		struct ReadyQueueNode* current = head;
		struct ReadyQueueNode* next;
		if (current != NULL){
			next = current->next;
			free(current);
			current = next;
		}
		
		head = NULL;
		tail = NULL;

		return 0;
	}

	scheduler();
	
	return 0;
}
// interpreter function
int interpreter(char *words[]){
	int errCode = 0;
	if (strcmp(words[0], "help") == 0) { errCode = help(); }
	else if (words[0] == NULL) { return 0; }
	else if (strcmp(words[0], "set") == 0) { errCode = set(words); }
	else if (strcmp(words[0], "quit") == 0) { errCode = quit(); }
	else if (strcmp(words[0], "print") == 0) { errCode = print(words); }
	else if (strcmp(words[0], "run") == 0) { errCode = run(words); }
	else if (strcmp(words[0], "exec") == 0) { errCode = exec(words); }
	else if (strcmp(words[0], "") == 0) { errCode = 0; }
	else {
		printf("Unknown command\n");
		errCode = 0;
	}
	return errCode;
}
