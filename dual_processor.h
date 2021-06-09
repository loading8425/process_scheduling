//
//  Dual_processor.h
//  Comp30023_P1
//
//  Created by MINGYANG on 22/3/21.
//

#ifndef dual_processor_h
#define dual_processor_h

#include <stdio.h>
#include "process.h"

int get_remainTime(CPU_t *cpu);

//return a set of sub processes
process_t **create_subprocess(process_t *parent, int num_cpu, process_t *sub_process[]);

#endif /* dual_processor_h */
