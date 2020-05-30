#ifndef CPU_H
#define CPU_H

struct CPU{
	int IP;
	char IR[1000];
	int quanta;
};

struct CPU* cpu;

void run(int quanta);

#endif
