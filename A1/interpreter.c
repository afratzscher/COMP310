#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "shellmemory.h"

int fileOpen = 0; //flag indicating if file open (because handle quit differently in script or shell)
FILE *p; //file pointer for script to run

// help function prints list of commands and descriptions 
int help(){
	printf("COMMAND		 	   	 DESCRIPTION\n");
	printf("help			 	 Displays all commands\n");
	printf("quit				 Exits/terminates the shell with 'Bye!'\n");
	printf("set VAR STRING			 Assigns a value to shell memory\n");
	printf("print VAR			 Displays the STRING assigned to VAR\n");
	printf("run SCRIPT.TXT 			 Executes the file SCRIPT.TXT\n");
	return 0;
}

// quit function: if in script, exit script and go back to shell; if in shell, exit shell
int quit(){
	// if file open, exit script and return to shell
	if (fileOpen == 1){
		printf("Bye!\n");
		return 0;
	}
	// else, exit shell
	printf("Bye!\n");
	printf("");
	return -1;
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

// run function opens file and executes commands line by line
// if file doesnt exist, return to shell
int run(char *words[]) {
	int errCode = 0;
	char line[1000];
	p = fopen(words[1], "rt");
	
	// if opening fails OR stack overflow from infinite recursion, have script not found printed
	if (p == NULL){
	       printf("Script not found\n");
	       return 0;
	}

	//flag for open file
	fileOpen = 1; 

	fgets(line, 999, p);
	while(!feof(p)){
		errCode = parse(line);
		if (errCode != 0){
			fclose(p);
			return errCode;
		}
		fgets(line,999,p);
	}

	fclose(p);

	//flag for closed file
	fileOpen = 0;
	return errCode;
}

// interpreter function
int interpreter(char *words[]){
	int errCode = 0;
	if (strcmp(words[0], "help") == 0) { errCode = help(); }
	else if (strcmp(words[0], "set") == 0) { errCode = set(words); }
	else if (strcmp(words[0], "quit") == 0) { errCode = quit(); }
	else if (strcmp(words[0], "print") == 0) { errCode = print(words); }
	else if (strcmp(words[0], "run") == 0) { errCode = run(words); }
	else if (strcmp(words[0], "") == 0) { errCode = quit(); }
	else {
		printf("Unknown command\n");
		errCode = 0;
	}
	return errCode;
}
