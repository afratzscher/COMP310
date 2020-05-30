#include "pcb.h"

#ifndef READYQUEUE_H
#define READYQUEUE_H

struct ReadyQueueNode{
	PCB pcb;
	struct ReadyQueueNode *next;
};

struct ReadyQueueNode* head;
struct ReadyQueueNode* tail;

#endif
