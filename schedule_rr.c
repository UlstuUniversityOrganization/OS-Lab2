#include "schedulers.h"
#include "list.h"
#include "stdlib.h"
#include "cpu.h"
#include "sys/param.h"
#include "stdio.h"
struct node* tasks = NULL;
struct node* selected = NULL;

void add(Task* task) {
    insert(&tasks, task);
}

Task* pickNext() {
    if (tasks == NULL) {
        return NULL;
    }
    Task* curTask = tasks->task;

    if (curTask != NULL) {
        struct node* curNode = tasks;
        struct node* nextNode = curNode->next;

        while (curNode->next != NULL) {
            curNode = curNode->next;
        }

        curNode->next = tasks;
        tasks = nextNode;
        curNode->next->next = NULL;
    }
    return curTask;
}

void schedule() {
    Task* curTask;

    while ((curTask = pickNext()) != NULL || tasks != NULL) {
        run(curTask, MIN(curTask->burst, QUANTUM));
        curTask->burst -= MIN(curTask->burst, QUANTUM);

        if (curTask->burst == 0 && tasks != NULL) {
            delete(&tasks, curTask);
        }
    }
}
