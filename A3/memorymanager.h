#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H
int countTotalPages(FILE *f);
void loadPage(int pageNumber, FILE *f, int frameNumber);
int findFrame();
int findVictim(PCB *p);
int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);
void callLoad(int numPages, int framestart, FILE *fp, PCB *p, int victim, int pageNumber);
int launcher(FILE *p);
#endif
