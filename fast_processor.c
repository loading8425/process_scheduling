//
//  fast_processor.c
//  Comp30023_P1
//
//  Created by Loading on 27/3/21.
//

#include "fast_processor.h"
#include "dual_processor.h"

void fast_processor(process_t *process_head, int total_process, int time, int num_CPU){
    int num_process = 0;
    int total_time_need = 0;
    double overhead[total_process];
    int O_index = 0;
    
    
    //set up cpu list
    CPU_t *cpu_list[num_CPU];
    for(int i=0; i<num_CPU; i++){
        cpu_list[i] = NULL;
    }
    
    process_t *process = process_head;
    
    while(1){

        //When all list clear, stop simulator
        if(process->next == NULL){
            int break_flag = 0;
            for(int i=0; i<num_CPU; i++){
                if(cpu_list[i] != NULL)
                    if(cpu_list[i]->current_process->flag != 1){
                        break_flag = 0;
                        break;
                    }
                break_flag = 1;
            }
            if(break_flag == 1)
                break;
        }
        
        //Add process into cpus
        if(process != NULL){
            int K = 0;
            int CurrentTimeProcess = 0;
            process_t *p = process;
            double AverageTime = 0;
            while(p != NULL && p->reachTime == time){
                CurrentTimeProcess++;
                AverageTime = AverageTime + p->remainTime;
                p = p->next;
            }
            p = process;
            if(AverageTime == 0)
                AverageTime = 99999999;
            AverageTime = AverageTime/CurrentTimeProcess;
            
            while(p != NULL && p->reachTime == time){
                if(p->remainTime > AverageTime){
                    p->parallelisable = 1;
                }else{
                    p->parallelisable = 0;
                }
                p = p->next;
            }
    
            while(process->reachTime == time){
                //paralle case
                if(process->parallelisable == 1){
    
                    process_t *sub_process[K];
                    create_subprocess(process, K, sub_process);
                    num_process ++;

                    //create node for sub-process
                    for(int i=0; i<K; i++){
                        CPU_t *tmp = (CPU_t*)malloc(sizeof(CPU_t));
                        tmp->current_process = sub_process[i];
                        tmp->next = NULL;
                        int min_time = 0;
                        for(int x=1; x<num_CPU; x++){
                            if(get_remainTime(cpu_list[x]) < get_remainTime(cpu_list[min_time])){
                                min_time = x;
                            }
                        }
                        cpu_list[min_time]=insert_node(cpu_list[min_time], tmp);
                    }
                    
                }else{
                //None paralle case
                    //choose shortest CPU
                    int min_time = 0;
                    for(int i=1; i<num_CPU; i++){
                        if(get_remainTime(cpu_list[i]) < get_remainTime(cpu_list[min_time])){
                            min_time = i;
                        }
                    }
                    //add process into cpu
                    CPU_t *tmp_node = (CPU_t*)malloc(sizeof(CPU_t));
                    tmp_node->current_process = process;
                    tmp_node->next = NULL;
                    num_process++;

                    CPU_t *previous = cpu_list[min_time];
                    cpu_list[min_time] = insert_node(cpu_list[min_time], tmp_node);
                    
                    //if previous running process != current process, previous stop running
                    if(previous != NULL){
                        if(previous != cpu_list[min_time]){
                            previous->current_process->running = 0;
                        }
                    }
                }
                //next process
                if(process->next == NULL)
                    break;
                process = process->next;
            }
        }
        
        //print running process
        for(int i=0; i<num_CPU; i++){
            if(cpu_list[i] != NULL){
                if(cpu_list[i]->current_process->running == 0 && cpu_list[i]->current_process->flag == 0){
                    print_start_status(cpu_list[i], time, i);
                    cpu_list[i]->current_process->running = 1;
                }
                
                cpu_list[i]->current_process->remainTime--;
                
                if(cpu_list[i]->current_process->remainTime == 0){
                    if(cpu_list[i]->current_process->parallelisable == 0){
                        num_process--;
                    }
                }
            }
        }
        
        //time passed
        time++;
        
        //process finished runing
        for(int i=0; i<num_CPU; i++){
            if(cpu_list[i] != NULL){
                if(cpu_list[i]->current_process->remainTime == 0){
                    
                    //set finish status
                    cpu_list[i]->current_process->flag = 1;
                    print_finish_status(cpu_list[i], &num_process, time);
                    cpu_list[i]->current_process->running = 0;
                    
                    if(cpu_list[i]->next != NULL)
                        cpu_list[i]->next->current_process->running = 0;
                    
                    //calculate performance
                    if(cpu_list[i]->current_process->parallelisable>=2){
                        int sub_flag = 0;
                        process_t *tmp = cpu_list[i]->current_process->sub_parent->sub_head;
                        while(tmp != NULL){
                            if(tmp->flag == 0){
                                sub_flag = 0;
                                break;
                            }
                            sub_flag = 1;
                            tmp = tmp->next;
                        }
                        if(sub_flag == 1){
                            total_time_need += time - cpu_list[i]->current_process->reachTime;
                            overhead[O_index] = (double)(time - cpu_list[i]->current_process->reachTime) / (double)cpu_list[i]->current_process->exeTime;
                            O_index++;
                        }
                    }else{
                        total_time_need += time - cpu_list[i]->current_process->reachTime;
                        overhead[O_index] = (double)(time - cpu_list[i]->current_process->reachTime) / (double)cpu_list[i]->current_process->exeTime;
                        O_index++;
                    }
                    
                    
                    //Next run
                    if(cpu_list[i]->next == NULL){
                        cpu_list[i]->current_process->remainTime = 999999999;
                        cpu_list[i]->current_process->flag = 1;
                    }else{
                        CPU_t *tmp = cpu_list[i];
                        cpu_list[i] = cpu_list[i]->next;
                        cpu_list[i]->current_process->running = 0;
                        free(tmp);
                    }
                    
                }
            }
        }
        
    }
    
    //print Performance statistics
    double max = 0;
    double average = 0;
    int turnaround = 0;
    for(int i=0; i<total_process; i++){
        average += overhead[i];
        if(overhead[i]>max)
            max = overhead[i];
    }
    max = max * 100;
    max = round(max)/100;
    average = average/total_process * 100;
    average = round(average)/100;
    turnaround = ceil((double)total_time_need/(double)total_process);
    printf("Turnaround time %d\n", turnaround);
    printf("Time overhead %g %g\n", max, average);
    printf("Makespan %d\n", time);
}
