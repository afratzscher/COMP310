#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include "shell.h"
#include "shellmemory.h"

int main(){
	char prompt[100] = {'$', '\0'};
	char *userInput = malloc(sizeof(char) * 1000);
	int errorCode = 0;
	
	init();

	printf("Welcome to the AnneSophie Fratzscher shell!\n");
	printf("Version 1.0 Created January 2020\n");
	
	while(1){
		printf("%s ", prompt);
		fgets(userInput,999, stdin);
		errorCode = parse(userInput);	
		if (errorCode==-1) { 
			free(userInput);
			exit(99); 
		}
	} 
}

int parse(char ui[]){
	char tmp[200];
	char *words[100];
	for (int i = 0; i < 99; i++){
		words[i] = malloc(sizeof(char));
	}

	int a,b;
	int w = 0;

	for (a = 0; ui[a]==' ' && a<1000; a++); //skip white spaces
	
	while(ui[a] != '\0' && a<1000){
		for (b = 0; ui[a] != '\0' && ui[a] != ' ' && a<1000 && ui[a] != '\n'; a++, b++){	
			tmp[b] = ui[a];
		}
		tmp[b] = '\0';
		words[w] = strdup(tmp);
		a++; w++;
	}
	int errCode = interpreter(words);
	return errCode;
}
