//  Created by Mingyang Liu on 19/3/21.
//
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "read_txt.h"
#include "process.h"
#include "dual_processor.h"
#include "multi_processor.h"
#include "fast_processor.h"

int main(int argc,char **argv){
    // Read Command line
    int ch;
    opterr = 0;
    char *filename = NULL;
    int numCPU = 0;
    int time = 0;
    int Cflag = 0;
    int total_process = 0;
    
    while((ch = getopt(argc,argv,"cf:p:"))!= -1){
        switch(ch){
            case 'f':
                filename = (char*) malloc(strlen(optarg)+1);
                strcpy(filename, optarg);
                break;
            case 'p':
                numCPU = atoi(optarg);
                break;
            case 'c':
                Cflag = 1;
                break;
            default:
                 break;
        }
    }
    
    //get file pointer
    FILE *fp = fopen(filename, "r");
    if(!fp){
        fprintf(stderr,"file not found");
        exit(0);
    }
    
    //load file to linked list
    process_t *first_process = NULL;
    first_process = load_file(fp, first_process, &total_process);
    fclose(fp);
    
    //Challenge task
    if(Cflag == 1){
        fast_processor(first_process, total_process, time, numCPU);
    }
    
    //sort list
    process_t tmp;
    process_t *t = &tmp;
    t->next = first_process;
    sort_list(&t);
    first_process = t->next;

    //Multiple CPUs
    if(Cflag == 0)
        multi_processor(numCPU, time, first_process, total_process);
    
    //free memory
    while(first_process != NULL){
        process_t *tmp = first_process;
        first_process = first_process->next;
        free(tmp);
    }
    
    free(filename);
    return 0;
}
