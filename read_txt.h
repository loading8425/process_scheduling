//  Created by Mingyang Liu on 19/3/21.
#ifndef read_txt_h
#define read_txt_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "process.h"

//read one word from file
char* read_line(FILE *fp, char *buff);

//Load process to a linked list
struct process_struct* load_file(FILE *fp, process_t *first_process, int *total_process);

//print status
void print_start_status(CPU_t *node, int time, int CPU);
void print_finish_status(CPU_t *node, int *num_process, int time);

//sort list
void sort_list(process_t **L);

#endif
