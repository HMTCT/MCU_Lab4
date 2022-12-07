/*
 * scheduler.c
 *
 *  Created on: Nov 11, 2022
 *      Author: HMT
 */

#include "scheduler.h"
#include "queue.h"

#define	QUEUE	1	//change this to 0 if using array structure

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;
struct Queue* q = NULL;

void SCH_Init(void){
	current_index_task = 0;
	q = createQueue();
}

void SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	if(!QUEUE && current_index_task < SCH_MAX_TASKS){
		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period =  PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;
		return;
	}

	if (q->size < SCH_MAX_TASKS){
		sTasks task;
		task.pTask = pFunction;
		task.Delay = DELAY;
		task.Period =  PERIOD;
		task.RunMe = 0;
		task.TaskID = current_index_task;
		current_index_task++;

		enQueue(q, task);
	}
}

void SCH_Update(void){
	if (!QUEUE){
		for (int i = 0; i < current_index_task; ++i) {
			if (SCH_tasks_G[i].Delay > 0){
				SCH_tasks_G[i].Delay--;
			}
			else{
				SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period;
				SCH_tasks_G[i].RunMe += 1;
			}
		}
		return;
	}

	if (q->size == 0)
		return;

	if (q->front->key.Delay){
		q->front->key.Delay--;
	}

	if (q->front->key.Delay <= 0){
		q->front->key.RunMe+=1;
	}
}

void SCH_Dispatch_Tasks(void){
	if (!QUEUE){
		for (int i = 0; i < current_index_task; ++i){
			if (SCH_tasks_G[i].RunMe > 0){
				SCH_tasks_G[i].RunMe--;
				(*SCH_tasks_G[i].pTask)();
			}
		}
		return;
	}

	if (q->size == 0)
		return;

	while (q->front->key.RunMe){
		(*q->front->key.pTask)();
		sTasks task = q->front->key;
		task.Delay = task.Period;
		task.RunMe = 0;

		deQueue(q);
		if (task.Period)
			enQueue(q, task);
	}
}
void SCH_Delete_Task(uint32_t taskID){
	if (!QUEUE){
		if (taskID < 0 || taskID > current_index_task)
			return;

		SCH_tasks_G[taskID].pTask = 0x0000;
		SCH_tasks_G[taskID].Delay = 0;
		SCH_tasks_G[taskID].Period = 0;
		SCH_tasks_G[taskID].RunMe = 0;

		for (int i = taskID; i < current_index_task - 1; ++i) {
			SCH_tasks_G[i] = SCH_tasks_G[i + 1];
		}

		current_index_task--;
		return;
	}

	if (taskID < 0 || taskID >= q->size)
		return;

	q->size--;
	struct QNode* prev = NULL;
	struct QNode* it = q->front;
	for (int i = 0; i < taskID; ++i) {
		prev = it;
		it = it->next;
	}
	if (prev)
		prev->next = it->next;

	else{
		q->front = q->front->next;
		if (!q->front)
			q->rear = NULL;
	}

	if (it == q->rear){
		q->rear = prev;
		q->total_delay -= it->key.Delay;
	}

	if (q->front && it->next)
		it->next->key.Delay += it->key.Delay;
	free(it);
}

