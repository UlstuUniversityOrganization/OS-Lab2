#include "schedulers.h"
#include "list.h"
#include "stdlib.h"
#include "cpu.h"
#include "sys/param.h"
#include "string.h"

struct node* tasks = NULL;

void add(Task* task) {
    insert(&tasks, task);
}

Task* pickNext() {
    if (tasks == NULL) {
        return NULL;
    }
    struct node* curNode = tasks;
    Task* curTask = tasks->task;

    while (curNode != NULL) {
        if (curNode->task->priority > curTask->priority) {
            curTask = curNode->task;
        }

        curNode = curNode->next;
    }

    return curTask;
}

void schedule() {
    Task* curTask = NULL;

    while ((curTask = pickNext()) != NULL || tasks != NULL) {
        int execTime = MIN(curTask->burst, QUANTUM);
        run(curTask, execTime);
        curTask->burst -= MIN(curTask->burst, QUANTUM);

        struct node* current = tasks;
        while (current != NULL) {
            if (curTask->priority == current->task->priority && strcmp(curTask->name, current->task->name) != 0) {
                int execTime = (current->task->burst < QUANTUM) ? current->task->burst : QUANTUM;
                run(current->task, execTime);
                current->task->burst -= MIN(current->task->burst, QUANTUM);

                if (current->task->burst == 0 && tasks != NULL) {
                    delete(&tasks, current->task);
                }
            }

            current = current->next;
        }

        if (curTask->burst == 0 && tasks != NULL) {
            delete(&tasks, curTask);
        }
    }
}
