//
//  multi_processor.h
//  Comp30023_P1
//
//  Created by Mingyang on 24/3/21.
//

#ifndef multi_processor_h
#define multi_processor_h

#include <stdio.h>
#include "process.h"
#include "read_txt.h"


void multi_processor(int num_CPU, int time, process_t* process_head, int total_process);

#endif /* multi_processor_h */
