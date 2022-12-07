/*
 * queue.h
 *
 *  Created on: Dec 6, 2022
 *      Author: HMT
 */

#ifndef INC_QUEUE_H_
#define INC_QUEUE_H_

#include <scheduler.h>
#include <stdlib.h>

// A linked list (LL) node to store a queue entry
struct QNode {
    sTasks key;
    struct QNode* next;
};

// The queue, front stores the front node of LL and rear
// stores the last node of LL
struct Queue {
    struct QNode *front, *rear;
    int size;
    int total_delay;
};

struct QNode* newNode(sTasks k);
struct Queue* createQueue();
void enQueue(struct Queue* q, sTasks k);
void deQueue(struct Queue* q);

#endif /* INC_QUEUE_H_ */
