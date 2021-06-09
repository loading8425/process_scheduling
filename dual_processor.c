//
//  Dual_processor.c
//  Comp30023_P1
//
//  Created by MINGYANG on 22/3/21.
//

#include "dual_processor.h"
#include "read_txt.h"

int get_remainTime(CPU_t *cpu){
    if(cpu==NULL)
        return 0;
    
    CPU_t *tmp = cpu;
    int remainTime = 0;
    
    while(tmp != NULL){
        if(tmp->current_process->flag == 0)
            remainTime = remainTime + tmp->current_process->remainTime;
        tmp = tmp->next;
    }
    return remainTime;
}

process_t **create_subprocess(process_t *parent, int sub_num, process_t *sub_process[]){
    int new_time = ceil(((double)parent->remainTime / (double)sub_num) + 1);
    int paralle = 2;
    for(int i=0; i<sub_num; i++){
        sub_process[i] = NULL;
    }
    
    for(int i=0; i<sub_num; i++){
        process_t *sub = (process_t*)malloc(sizeof(process_t));
        sub->flag = 0;
        sub->parallelisable = paralle++;
        sub->pid = parent->pid;
        sub->remainTime = new_time;
        sub->reachTime = parent->reachTime;
        sub->running = 0;
        sub->next = NULL;
        sub->sub_parent = parent;
        sub->exeTime = parent->exeTime;
        sub_process[i] = sub;
        if(i != 0){
            sub_process[i-1]->next = sub_process[i];
        }
    }
    
    parent->sub_head = sub_process[0];
    return sub_process;
}
