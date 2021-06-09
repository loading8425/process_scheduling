#ifndef process_h
#define process_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct process_struct{
    
    int reachTime;      //time arrival
    int exeTime;
    int remainTime;     //remaining time
    int running;        //chech if this process is runing, 0 is not 1 is
    int pid;            //process id
    int flag;           //check if this process is done, 0 is not 1 is done
    int parallelisable; //check if this process is parallelisable, 0 is not 1 is, 2 means it is a sub-process
    
    struct process_struct* sub_parent;
    struct process_struct* sub_head;
    struct process_struct* next;
}process_t;

typedef struct CPU{
    process_t *current_process;
    struct CPU *next;
}CPU_t;

//return head of new list after insertion
CPU_t* insert_node(CPU_t *head, CPU_t *node);

#endif /* process_h */
