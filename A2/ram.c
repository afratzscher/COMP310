#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *ram[1000];
int numPrograms = 0;

void addToRAM(FILE *p, int *start, int *end){
	int i;
	for (i = 0; i < 1000; i++){
		if (ram[i] == NULL){
			break;
		}
		i++;
	}

	char line[1000];
	*start = i;

	fgets(line,999, p);	
	while(!feof(p)){
		if (i > 1000){
			printf("Not enough space left in RAM\n");
			i = 0;
			break;
		}
		ram[i] = strdup(line);
		fgets(line,999,p);
		i++;
	}
	*end = i-1;
	numPrograms++;
}

void clearFromRAM(int start, int end){
	for (int i = start; i <= end; i++){
		ram[i] = NULL;
	}
	numPrograms--;
}

void clearAllRAM(){
	for (int i = 0; i < 1000; i++){
		ram[i] = NULL;
	}
	numPrograms = 0;
}
