//  Created by Mingyang Liu on 19/3/21.
//
#include "read_txt.h"
#include "dual_processor.h"

char* read_line(FILE *fp, char* buff){
    memset(buff, 0, 32);
    fgets(buff, 32, fp);
    return buff;
}

struct process_struct* load_file(FILE *fp, process_t *first_process, int *total_process){
    process_t *process = NULL;
    char buff[32];
    while(!feof(fp)){
        process_t *p = (process_t*)malloc(sizeof(process_t));
        char tmp[4][32];
        
        for(int i=0; i<4; i++){
            memset(tmp[i], 0, 32);
        }
        int tmp_i = 0;
        int x = 0;
        read_line(fp, buff);
        for(int i=0; i<=strlen(buff); i++){
            if(buff[i] == ' '){
                x = 0;
                tmp_i++;
                continue;
            }
            tmp[tmp_i][x++] = buff[i];
        }
        
        p->reachTime = atoi(tmp[0]);
        p->remainTime = atoi(tmp[2]);
        p->exeTime = p->remainTime;
        p->pid = atoi(tmp[1]);
        p->flag = 0;
        p->running = 0;
        p->next = NULL;
        if(tmp[3][0] == 'n'){
            p->parallelisable = 0;
        }else{
            p->parallelisable = 1;
        }
        if(p->pid == 0 && p->remainTime == 0 && p->reachTime==0)
            break;
        *total_process += 1;
        
        if(first_process == NULL){
            first_process = p;
            process = first_process;
        }else{
            process->next = p;
            process = process->next;
        }
        
    }
    
    return first_process;
}

void print_start_status(CPU_t *node, int time, int CPU){
    if(node->current_process->parallelisable>=2){
        int sub_pid = node->current_process->parallelisable - 2;
        printf("%d,RUNNING,pid=%d.%d,remaining_time=%d,cpu=%d\n", time,node->current_process->pid, sub_pid,node->current_process->remainTime, CPU);
    }else{
        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time,node->current_process->pid, node->current_process->remainTime, CPU);
    }
}

void print_finish_status(CPU_t *node, int *num_process, int time){
    if(node->current_process->parallelisable>=2){
        process_t *tmp = node->current_process->sub_parent->sub_head;
        while(tmp != NULL){
            if(tmp->flag == 0){
                return;
            }
            tmp = tmp->next;
        }
        *num_process -= 1;
        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, node->current_process->pid,*num_process);
    }else{
        printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, node->current_process->pid, *num_process);
    }
}

void sort_list(process_t **L){
    process_t *tail = NULL;
    int change = 1;
    while(tail != (*L)->next && change == 1){
        process_t *pre = *L;
        process_t *pf = (*L)->next;
        process_t *ps = pf->next;
        change = 0;
        while(pf != tail && pf->next != tail){
            if(pf->remainTime > ps->remainTime){
                change = 1;
                pre->next = ps;
                pf->next = ps->next;
                ps->next = pf;
            }
            pre = pre->next;
            pf = pre->next;
            ps = pf->next;
        }
        tail = pf;
    }
    
    tail = NULL;
    change = 1;
    while(tail != (*L)->next && change == 1){
        process_t *pre = *L;
        process_t *pf = (*L)->next;
        process_t *ps = pf->next;
        change = 0;
        while(pf != tail && pf->next != tail){
            if(pf->reachTime > ps->reachTime){
                change = 1;
                pre->next = ps;
                pf->next = ps->next;
                ps->next = pf;
            }
            pre = pre->next;
            pf = pre->next;
            ps = pf->next;
        }
        tail = pf;
    }
}
