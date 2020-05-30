#ifndef RAM_H
#define RAM_H

char *ram[1000];
void addToRAM(FILE *p, int *start, int *end);
void clearFromRAM(int start, int end);
void clearAllRAM();

#endif
