/*
 * queue.c
 *
 *  Created on: Dec 6, 2022
 *      Author: HMT
 */

#include "queue.h"

// A utility function to create a new linked list node.
struct QNode* newNode(sTasks k){
    struct QNode* temp
        = (struct QNode*)malloc(sizeof(struct QNode));
    temp->key = k;
    temp->next = NULL;
    return temp;
}

// A utility function to create an empty queue
struct Queue* createQueue(){
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    q->size = 0;
    q->total_delay = 0;
    return q;
}

// The function to add a key k to q
void enQueue(struct Queue* q, sTasks k){
    // Create a new LL node
    struct QNode* temp = newNode(k);
    q->size++;
    q->total_delay+=1;
    // If queue is empty, then new node is front and rear
    // both
    if (q->front == NULL) {
        q->front = q->rear = temp;
        q->total_delay = temp->key.Delay;
        return;
    }

    if (temp->key.Delay >= q->total_delay){
    	//Insert at the end of queue
    	temp->key.Delay -= q->total_delay;

    	if (temp->key.Delay == 0)
    		temp->key.RunMe+=1;

    	q->total_delay += temp->key.Delay;
		q->rear->next = temp;
		q->rear = temp;
		return;
    }

    //Insert the new node behind the node which has
    //the same or smaller delay value with it
    //The delay time of nodes behind this new node
    //will be updated

    struct QNode* prev = NULL;
    struct QNode* it = q->front;
    while (it){
    	if (it->key.Delay >= temp->key.Delay){
    		if (!prev){
    			//Insert at head of queue
    			temp->next = q->front;
    			q->front = temp;
    		}
    		else{
    			prev->next = temp;
    			temp->next = it;
    		}

    		if (it->key.Delay == temp->key.Delay){
    			it->key.Delay = 0;
    			it->key.RunMe++;
    			return;
    		}

    		//Update delay time of remaining node
    		it->key.Delay -= temp->key.Delay;
    		while (it){
    			it->key.Delay -= 1;
    			it = it->next;
    		}
    		return;
    	}

    	temp->key.Delay -= it->key.Delay + 1;
    	prev = it;
    	it = it->next;
    }

}

// Function to remove a key from given queue q
void deQueue(struct Queue* q){
    // If queue is empty, return NULL.
    if (q->front == NULL)
        return;

    q->size--;

    // Store previous front and move front one node ahead
    struct QNode* temp = q->front;
    q->front = q->front->next;
    q->total_delay -= 1;

    // If front becomes NULL, then change rear also as NULL
    if (q->front == NULL)
        q->rear = NULL;

    free(temp);
}
