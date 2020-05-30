#ifndef SHELLMEM_H
#define SHELLMEM_H

void shell_mem_init();
void shell_mem_destroy();
int setVar(char *name, char *val);
int printVal(char *name);

#endif
