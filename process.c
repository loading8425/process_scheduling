//
//  multi_processor.c
//  Comp30023_P1
//
//  Created by Mingyang on 24/3/21.
//

#include "process.h"
#include "read_txt.h"

#define CPU 0

CPU_t* insert_node(CPU_t *head, CPU_t *node){
    CPU_t *p = head;
    CPU_t *previous = NULL;
    
    //security check
    if(node->current_process == NULL){
        printf("eroor insert: node is empty");
        exit(0);
    }
    
    if(head == NULL){
        node->next = NULL;
        return node;
    }
    
    if(node->current_process->remainTime < p->current_process->remainTime){
    //first case: node is smallest
        node->next = head;
        return node;
    }
        
    if(node->current_process->remainTime == p->current_process->remainTime){
    //Seconde case: node is equal compare pid
        //pid smaller
        if(node->current_process->pid < p->current_process->pid){
            node->next = head;
            return node;
        }
        //pid bigger
        while(1){
            if(p->next == NULL){
                p->next = node;
                node->next = NULL;
                return head;
            }
            previous = p;
            p = p->next;
            if(node->current_process->pid < p->current_process->pid){
                previous->next = node;
                node->next = p;
                return head;
            }
        }
    }

    //Third case: node is greater
    while(1){
        if(p->next == NULL){
            p->next = node;
            node->next = NULL;
            return head;
        }
        previous = p;
        p = p->next;
        if(node->current_process->remainTime < p->current_process->remainTime){
            previous->next = node;
            node->next = p;
            return head;
        }else if(node->current_process->remainTime == p->current_process->remainTime){
            //find the node have same remaining time, compare pid
            if(node->current_process->pid < p->current_process->pid){
                previous->next = node;
                node->next = p;
                return head;
            }
        }
    }
}

