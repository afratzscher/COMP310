#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// memory is array of struct
// NOTE: array can hold max 100 values

struct MEM {
	char *var;
	char *value;
};

int size; // holds index in which to add next var

struct MEM array[100];

void init(){
	size = 0;
	for (int i = 0; i < 99; i++){
		array[i].var = malloc(30*sizeof(char));
		array[i].value = malloc(30*sizeof(char));
	}
}

int setVar(char *name, char *val){
	//for loop to check
	for (int i = 0; i < size; i++){
		if (strcmp(array[i].var, name) == 0){
			array[i].value = val;
			return 0;
		}
	}

	// else, add to end of array
	array[size].var = name;
	array[size].value = val;
	size++;
	return 0;
}

int printVal(char *name){
	for (int i = 0; i < size; i++){
		if (strcmp(array[i].var, name) == 0){
			printf("%s", array[i].value);
			printf("\n");
			return 0;
		}
	}
	
	printf("Variable does not exist\n");
	return 0;
}
